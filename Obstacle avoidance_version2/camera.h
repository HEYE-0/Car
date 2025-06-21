#ifndef CAMERA_H
#define CAMERA_H

#include <opencv2/opencv.hpp>

class Camera {
public:
    Camera();
    ~Camera();
    void startCapture();

private:
    cv::VideoCapture cap;
};

#endif
