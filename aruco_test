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

    Ptr<aruco::Dictionary> dictionary = aruco::getPredefinedDictionary(aruco::DICT_4X4_50);
    aruco::DetectorParameters params;

    cout << "🎯 Starting ArUco detection... Press ESC to quit.\n";

    while (true) {
        Mat frame;
        cap >> frame;
        if (frame.empty()) continue;

        vector<int> ids;
        vector<vector<Point2f>> corners;
        aruco::detectMarkers(frame, dictionary, corners, ids, &params);

        if (!ids.empty()) {
            aruco::drawDetectedMarkers(frame, corners, ids);

            for (size_t i = 0; i < ids.size(); i++) {
                Point2f center(0, 0);
                for (const auto& pt : corners[i]) center += pt;
                center /= 4.0f;

                circle(frame, center, 5, Scalar(0, 255, 0), -1);
                putText(frame, "ID: " + to_string(ids[i]),
                        Point(center.x - 20, center.y - 20),
                        FONT_HERSHEY_SIMPLEX, 0.6, Scalar(0, 255, 0), 2);

                cout << "✅ Detected ID: " << ids[i]
                     << " at center (" << center.x << ", " << center.y << ")\n";
            }
        } else {
            cout << "🚫 No marker detected.\n";
        }

        imshow("ArUco Detection", frame);
        if (waitKey(10) == 27) break;  // ESC to exit
    }

    cap.release();
    destroyAllWindows();
    return 0;
}
