#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>
#include <thread>
#include <chrono>

using namespace std;
using namespace cv;

int main() {
    cout << "🎯 Starting periodic ArUco marker detection...\n";

    // 打开摄像头
    VideoCapture cap(0); // 改为你的摄像头编号
    if (!cap.isOpened()) {
        cerr << "❌ Failed to open camera\n";
        return -1;
    }

    // 使用 5x5 的 ArUco 字典，包含 ID 为 5 的图案
    Ptr<aruco::Dictionary> dictionary = aruco::getPredefinedDictionary(aruco::DICT_5X5_50);
    Ptr<aruco::DetectorParameters> parameters = aruco::DetectorParameters::create();

    while (true) {
        Mat frame;
        cap >> frame;
        if (frame.empty()) {
            cerr << "⚠️  Empty frame captured.\n";
            continue;
        }

        vector<int> ids;
        vector<vector<Point2f>> corners;
        aruco::detectMarkers(frame, dictionary, corners, ids, parameters);

        if (!ids.empty()) {
            cout << "✅ Detected marker IDs: ";
            for (int id : ids) {
                cout << id << " ";
            }
            cout << endl;
        } else {
            cout << "❌ No marker detected.\n";
        }

        // 每 3 秒检测一次
        this_thread::sleep_for(chrono::seconds(3));
    }

    return 0;
}
