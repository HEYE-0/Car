#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>
#include <thread>
#include <chrono>

using namespace std;
using namespace cv;

int main() {
    cout << "🎯 Starting periodic ArUco marker detection...\n";

    // Open camera
    VideoCapture cap(0); // Change to your camera index
    if (!cap.isOpened()) {
        cerr << "❌ Failed to open camera\n";
        return -1;
    }

    // Use 5x5 ArUco dictionary containing marker with ID 5
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

        // Detect every 3 seconds
    }

    return 0;
}
