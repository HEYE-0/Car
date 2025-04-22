#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>
#include <iostream>
#include <thread>
#include <chrono>

using namespace std;
using namespace cv;

int main() {
    cout << "🎯 Starting periodic ArUco detection (DICT_5X5_50, ID=5)..." << endl;

    // 打开默认摄像头（编号为0）
    VideoCapture cap(0);
    if (!cap.isOpened()) {
        cerr << "❌ Failed to open camera." << endl;
        return -1;
    }

    // 初始化5x5的aruco字典
    Ptr<aruco::Dictionary> dictionary = aruco::getPredefinedDictionary(aruco::DICT_5X5_50);
    aruco::DetectorParameters parameters;

    while (true) {
        Mat frame;
        cap >> frame;

        if (frame.empty()) {
            cerr << "⚠️ Empty frame, skipping." << endl;
            this_thread::sleep_for(chrono::seconds(3));
            continue;
        }

        // 检测二维码
        vector<int> ids;
        vector<vector<Point2f>> corners;
        aruco::detectMarkers(frame, dictionary, corners, ids, parameters);

        bool found_target = false;
        for (int id : ids) {
            if (id == 5) {
                found_target = true;
                break;
            }
        }

        if (found_target) {
            cout << "✅ Detected ArUco marker with ID 5!" << endl;
        } else {
            cout << "❌ Marker ID 5 not found." << endl;
        }

        // 每隔3秒检测一次
        this_thread::sleep_for(chrono::seconds(3));
    }

    return 0;
}
