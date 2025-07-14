#include "camera.h"
#include <iostream>
#include <thread>
#include <chrono>

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
        cap >> frame;
        if (frame.empty()) continue;

        int detectedId = -1;
        cv::Point2f position;
        bool found = detectMarker(frame, detectedId, position); // detectMarker is a placeholder for actual detection

        if (found && callback) {
            callback->onMarkerDetected(detectedId, position);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

bool Camera::detectMarker(cv::Mat &frame, int &id, cv::Point2f &pos) {
    // Example placeholder implementation; replace with actual ArUco detection
    // e.g. using cv::aruco::detectMarkers, etc.
    // if detected:
    //    id = detectedId;
    //    pos = detectedPosition;
    //    return true;
    return false;
}