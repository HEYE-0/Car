#ifndef CAMERA_H
#define CAMERA_H

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <thread>

class CameraEventInterface {
public:
    virtual void onMarkerDetected(int id, cv::Point2f pos) = 0;
    virtual ~CameraEventInterface() = default;
};


class Camera {
public:
 
    Camera();

    ~Camera();

    void registerCallback(CameraEventInterface* cb);

private:

    void processLoop();

    bool detectMarker(cv::Mat &frame, int &id, cv::Point2f &pos);

    cv::VideoCapture cap;
    CameraEventInterface* callback;
    std::thread worker;
    bool running;
};

#endif // CAMERA_H
