#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>
#include <iostream>
#include <thread>
#include <chrono>

using namespace std;
using namespace cv;

int main() {
    Ptr<aruco::Dictionary> dictionary = aruco::getPredefinedDictionary(aruco::DICT_5X5_50);

    while (true) {
        // 拍一张图像
        system("libcamera-still -n -o temp.jpg --width 640 --height 480");

        // 加载图像
        Mat frame = imread("temp.jpg");
        if (frame.empty()) {
            cout << "⚠️ Failed to load captured image\n";
            continue;
        }

        // 检测 ArUco
        vector<int> ids;
        vector<vector<Point2f>> corners;
        aruco::detectMarkers(frame, dictionary, corners, ids);

        if (!ids.empty()) {
            cout << "🎯 Detected ArUco IDs: ";
            for (int id : ids) cout << id << " ";
            cout << endl;
        } else {
            cout << "❌ No ArUco markers found\n";
        }

        this_thread::sleep_for(chrono::seconds(3)); // 每 3 秒检测一次
    }

    return 0;
}
