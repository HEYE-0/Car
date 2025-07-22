#include "camera.h"
#include <iostream>
#include <opencv2/aruco.hpp>

Camera::Camera() : callback(nullptr), running(true) {
    cap.open(0); // Open default camera /dev/video0
    if (!cap.isOpened()) {
        std::cerr << "[Camera] Failed to open camera device." << std::endl;
        running = false;
    } else {
        worker = std::thread(&Camera::processLoop, this);
    }
}

Camera::~Camera() {
    running = false;
    if (worker.joinable()) {
        worker.join();
    }
    if (cap.isOpened()) {
        cap.release();
    }
}

void Camera::registerCallback(CameraEventInterface* cb) {
    callback = cb;
}

void Camera::processLoop() {
    cv::Mat frame;
    while (running) {
        if (!cap.read(frame)) {
            std::cerr << "[Camera] Failed to read frame." << std::endl;
            continue;
        }

        int detectedId = -1;
        cv::Point2f position;
        bool found = detectMarker(frame, detectedId, position);

        if (found && callback) {
            callback->onMarkerDetected(detectedId, position);
        }
    }
}

bool Camera::detectMarker(cv::Mat& frame, int& id, cv::Point2f& pos) {
    // Example using OpenCV's ArUco detector
    std::vector<int> ids;
    std::vector<std::vector<cv::Point2f>> corners;

    static cv::Ptr<cv::aruco::Dictionary> dictionary = 
        cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_50);

    cv::aruco::detectMarkers(frame, dictionary, corners, ids);

    if (!ids.empty()) {
        id = ids[0];
        pos = corners[0][0];  // top-left corner of first marker
        return true;
    }
    return false;
}
