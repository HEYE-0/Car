#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>
#include <iostream>
#include <thread>
#include <chrono>

using namespace std;
using namespace cv;

int main() {
    cout << "🎯 Starting ArUco periodic detection..." << endl;

    VideoCapture cap(0);  // 打开摄像头
    if (!cap.isOpened()) {
        cerr << "❌ Cannot open camera" << endl;
        return -1;
    }

    Ptr<aruco::Dictionary> dictionary = aruco::getPredefinedDictionary(aruco::DICT_5X5_50);
    aruco::DetectorParameters params;

    while (true) {
        Mat frame;
        cap >> frame;

        if (frame.empty()) {
            cerr << "⚠️ Empty frame, skipping...\n";
            this_thread::sleep_for(chrono::seconds(3));
            continue;
        }

        // 检测 ArUco 标签
        vector<int> ids;
        vector<vector<Point2f>> corners;
        aruco::detectMarkers(frame, dictionary, corners, ids, &params);

        if (!ids.empty()) {
            cout << "✅ Detected marker IDs: ";
            for (int id : ids) cout << id << " ";
            cout << endl;
        } else {
            cout << "❌ No marker detected." << endl;
        }

        // 每隔 3 秒检测一次
        this_thread::sleep_for(chrono::seconds(3));
    }

    return 0;
}
