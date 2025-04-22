#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main() {
    VideoCapture cap(0);  // 默认摄像头
    if (!cap.isOpened()) {
        cerr << "❌ Cannot open camera" << endl;
        return -1;
    }

    Ptr<aruco::Dictionary> dictionary = aruco::getPredefinedDictionary(aruco::DICT_5X5_1000);
    aruco::DetectorParameters params;

    cout << "🎯 Starting headless ArUco detection... Press Ctrl+C to quit.\n";

    while (true) {
        Mat frame;
        cap >> frame;
        if (frame.empty()) continue;

        vector<int> ids;
        vector<vector<Point2f>> corners;
        aruco::detectMarkers(frame, dictionary, corners, ids, &params);

        if (!ids.empty()) {
            for (size_t i = 0; i < ids.size(); i++) {
                Point2f center(0, 0);
                for (const auto& pt : corners[i]) center += pt;
                center /= 4.0f;

                cout << "✅ Detected ID: " << ids[i]
                     << " at center (" << center.x << ", " << center.y << ")\n";
            }
        } else {
            cout << "🚫 No marker detected." << endl;
        }

        this_thread::sleep_for(chrono::milliseconds(300));  // 限制刷新频率
    }

    cap.release();
    return 0;
}
