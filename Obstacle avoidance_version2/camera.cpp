#include "camera.h"
#include <iostream>

Camera::Camera() {
    cap.open(1); // 默认打开 /dev/video0
    if (!cap.isOpened()) {
        std::cerr << "[Camera] Failed to open camera device." << std::endl;
    }
}

Camera::~Camera() {
    if (cap.isOpened()) {
        cap.release();
    }
    cv::destroyAllWindows();
}

void Camera::startCapture() {
    if (!cap.isOpened()) return;

    cv::Mat frame;
    while (true) {
        cap >> frame;
        if (frame.empty()) break;
        cv::imshow("Live Camera", frame);
        if (cv::waitKey(30) == 27) break; // ESC to exit
    }
}
