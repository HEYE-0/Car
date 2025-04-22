#include <iostream>
#include <thread>
#include <chrono>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <string>
#include <atomic>
#include <csignal>
#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>
#include <mutex>
#include <condition_variable>

using namespace std;
using namespace cv;

// --- 全局变量 ---
int serial_port;
atomic<bool> running(true);
atomic<char> mode('p');  // 初始为自动模式
atomic<bool> tagFound(false);

mutex mode_mutex;
condition_variable mode_cv;

// --- 串口初始化 ---
bool setupSerial(const string& port_name = "/dev/ttyACM0", int baud_rate = B9600) {
    serial_port = open(port_name.c_str(), O_RDWR);
    if (serial_port < 0) {
        perror("❌ Cannot open serial port");
        return false;
    }

    termios tty;
    tcgetattr(serial_port, &tty);
    tty.c_cflag = baud_rate | CS8 | CLOCAL | CREAD;
    tty.c_iflag = IGNPAR;
    tty.c_oflag = 0;
    tty.c_lflag = 0;
    tty.c_cc[VTIME] = 0;
    tty.c_cc[VMIN] = 1;
    tcflush(serial_port, TCIFLUSH);
    tcsetattr(serial_port, TCSANOW, &tty);

    return true;
}

void sendCommand(char cmd) {
    write(serial_port, &cmd, 1);
    cout << "➡️ Sent: " << cmd << endl;
}

void readArduino() {
    char buf[256];
    while (running) {
        int n = read(serial_port, buf, sizeof(buf) - 1);
        if (n > 0) {
            buf[n] = '\0';
            cout << buf;
            cout.flush();
        }
    }
}

// 摄像头跟踪线程（事件驱动）
void followAruco() {
    VideoCapture cap(0);
    if (!cap.isOpened()) {
        cerr << "❌ Cannot open camera" << endl;
        return;
    }

    Ptr<aruco::Dictionary> dictionary = aruco::getPredefinedDictionary(aruco::DICT_4X4_50);
    aruco::DetectorParameters params;
    int lost_counter = 0;
    const int max_lost = 60;
    char last_cmd = 'q';

    unique_lock<mutex> lock(mode_mutex);

    while (running) {
        mode_cv.wait(lock, [] { return mode == 'f' || !running; });
        if (!running) break;

        Mat frame;
        cap >> frame;
        if (frame.empty()) continue;

        vector<int> ids;
        vector<vector<Point2f>> corners;
        aruco::detectMarkers(frame, dictionary, corners, ids, &params);

        char cmd = 'q';

        if (!ids.empty()) {
            auto it = find(ids.begin(), ids.end(), 5);
            if (it != ids.end()) {
                int idx = distance(ids.begin(), it);
                Point2f center(0, 0);
                for (const auto& pt : corners[idx]) center += pt;
                center /= 4.0f;

                int frame_center = frame.cols / 2;
                int tolerance = 50;

                if (center.x < frame_center - tolerance) cmd = 'a';
                else if (center.x > frame_center + tolerance) cmd = 'd';
                else cmd = 'w';

                lost_counter = 0;
                tagFound = true;

                aruco::drawDetectedMarkers(frame, corners, ids);
                circle(frame, center, 6, Scalar(0, 255, 0), -1);
            } else {
                lost_counter++;
            }
        } else {
            lost_counter++;
        }

        if (lost_counter >= max_lost) {
            cmd = 'q';
            cout << "❌ target not found\n";
            tagFound = false;
        }

        if (cmd != last_cmd && mode == 'f') {
            sendCommand(cmd);
            last_cmd = cmd;
        }

        imshow("Tracking View", frame);
        if (waitKey(1) == 27) break;
    }

    cap.release();
    destroyWindow("Tracking View");
}

void enableKeyboardInput(termios& oldt) {
    termios newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

void restoreKeyboardInput(const termios& oldt) {
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

void handleSignal(int signal) {
    running = false;
    mode_cv.notify_all();
    close(serial_port);
    cout << "\n🚪 程序已退出\n";
    exit(0);
}

// --- 主函数 ---
int main() {
    signal(SIGINT, handleSignal);

    if (!setupSerial()) return 1;
    this_thread::sleep_for(chrono::seconds(2));

    sendCommand('p');
    cout << "🚗 默认进入自动模式 (p)，可切换 m=手动 f=跟随 ESC=退出\n";

    thread reader(readArduino);
    thread camera(followAruco);
    termios oldt;
    enableKeyboardInput(oldt);

    while (running) {
        char key;
        if (read(STDIN_FILENO, &key, 1) == 1) {
            if (mode == 'm' || mode == 'f') {
                switch (key) {
                    case 'w': case 'a': case 's': case 'd': case 'q':
                        sendCommand(key);
                        break;
                }
            }

            switch (key) {
                case 'p':
                    {
                        lock_guard<mutex> lock(mode_mutex);
                        mode = 'p';
                    }
                    mode_cv.notify_all();
                    sendCommand('p');
                    cout << "🔁 切换为自动避障模式\n";
                    break;
                case 'm':
                    {
                        lock_guard<mutex> lock(mode_mutex);
                        mode = 'm';
                    }
                    mode_cv.notify_all();
                    sendCommand('o');
                    cout << "🎮 手动控制模式\n";
                    break;
                case 'f':
                    {
                        lock_guard<mutex> lock(mode_mutex);
                        mode = 'f';
                    }
                    mode_cv.notify_all();
                    sendCommand('f');
                    cout << "👣 跟随模式启动 (摄像头识别)\n";
                    break;
                case 27:
                    running = false;
                    mode_cv.notify_all();
                    cout << "🛑 ESC 退出程序\n";
                    break;
            }
        }
        this_thread::yield(); // 非阻塞等待
    }

    reader.join();
    camera.join();
    restoreKeyboardInput(oldt);
    close(serial_port);
    return 0;
}
