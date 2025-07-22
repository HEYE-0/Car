#include "camera.h"
#include <iostream>

Camera::Camera() : callback(nullptr), running(true) {
    cap.open(0); // open default camera
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

        if (callback) {
            callback->onFrameCaptured(frame);
        }
    }
}
