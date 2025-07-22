#ifndef CAMERA_H
#define CAMERA_H

#include <opencv2/opencv.hpp>
#include <thread>


class CameraEventInterface {
public:
    virtual void onFrameCaptured(const cv::Mat& frame) = 0;
    virtual ~CameraEventInterface() = default;
};

class Camera {
public:
    Camera();
    ~Camera();

    void registerCallback(CameraEventInterface* cb);

private:
    void processLoop();

    cv::VideoCapture cap;
    CameraEventInterface* callback;
    std::thread worker;
    bool running;
};

#endif // CAMERA_H
