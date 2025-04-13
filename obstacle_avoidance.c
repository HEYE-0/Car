
#include <gpiod.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

using namespace std;
using namespace std::chrono_literals;

const char* chipname = "gpiochip4"; // 树莓派5 一般是 gpiochip4
const int IR_LEFT_PIN = 8;  // 请根据实际GPIO编号修改
const int IR_RIGHT_PIN = 9;

const char* SERIAL_PORT = "/dev/ttyUSB0"; // Arduino串口
int serial_fd;

gpiod_line* setup_input(gpiod_chip* chip, int line_num) {
    gpiod_line* line = gpiod_chip_get_line(chip, line_num);
    gpiod_line_request_input(line, "obstacle_sensor");
    return line;
}

void setupSerial() {
    serial_fd = open(SERIAL_PORT, O_RDWR | O_NOCTTY | O_NDELAY);
    if (serial_fd == -1) {
        cerr << "无法打开串口设备" << endl;
        exit(1);
    }

    struct termios options;
    tcgetattr(serial_fd, &options);
    cfsetispeed(&options, B9600);
    cfsetospeed(&options, B9600);
    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CRTSCTS;
    tcsetattr(serial_fd, TCSANOW, &options);
}

void sendCommand(const string& cmd) {
    write(serial_fd, cmd.c_str(), cmd.length());
}

int main() {
    gpiod_chip* chip = gpiod_chip_open_by_name(chipname);
    if (!chip) {
        cerr << "无法打开 GPIO 芯片接口！" << endl;
        return 1;
    }

    auto leftIR = setup_input(chip, IR_LEFT_PIN);
    auto rightIR = setup_input(chip, IR_RIGHT_PIN);
    setupSerial();

    while (true) {
        int left = gpiod_line_get_value(leftIR);
        int right = gpiod_line_get_value(rightIR);

        if (left == 0 && right == 0) {
            sendCommand("STOP\n");
            cout << "两侧有障碍，停止" << endl;
        } else if (left == 0) {
            sendCommand("RIGHT\n");
            cout << "左障碍，右转" << endl;
        } else if (right == 0) {
            sendCommand("LEFT\n");
            cout << "右障碍，左转" << endl;
        } else {
            sendCommand("FORWARD\n");
            cout << "前进" << endl;
        }

        this_thread::sleep_for(100ms);
    }

    gpiod_chip_close(chip);
    close(serial_fd);
    return 0;
}
