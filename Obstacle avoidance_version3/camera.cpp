#include "camera.h"
#include <iostream>

Camera::Camera() {
    cap.open(0); // Open default camera /dev/video0
    if (!cap.isOpened()) {
        std::cerr << "[Camera] Failed to open camera device." << std::endl;
    }
}

Camera::~Camera() {
    if (cap.isOpened()) {
        cap.release();
    }
}

bool Camera::readFrame(cv::Mat &frame) {
    if (!cap.isOpened()) return false;
    cap >> frame;
    return !frame.empty();
}