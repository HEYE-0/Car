#include <wiringPi.h>
#include <thread>
#include "robot.h"
#include "camera.h"

int main() {
    wiringPiSetup();

    // 初始化小车控制类
    Robot robot;

    // 初始化摄像头类
    Camera cam;

    // 启动摄像头图像捕捉线程
    std::thread camThread([&cam]() {
        cam.startCapture();  // 实时显示图像，ESC 退出
    });

    // 小车正常运行
    while (true) {
        robot.moveForward();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    camThread.join();  // 等待摄像头线程结束（正常不会到达）

    return 0;
}
