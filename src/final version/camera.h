#ifndef CAMERA_H
#define CAMERA_H

#include <opencv2/opencv.hpp>

class Camera {
public:
    Camera();
    ~Camera();

    // Used by Qt GUI to retrieve one frame
    bool readFrame(cv::Mat &frame);

private:
    cv::VideoCapture cap;
};

#endif