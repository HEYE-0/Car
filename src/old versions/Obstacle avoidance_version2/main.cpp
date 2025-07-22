#include <wiringPi.h>
#include <thread>
#include "robot.h"
#include "camera.h"

int main() {
    wiringPiSetup();

    // Initialize the robot control class
    Robot robot;

    // Initialize the camera class
    Camera cam;

    // Start the camera capture thread
    std::thread camThread([&cam]() {
        cam.startCapture();  // Real-time image display, exit with ESC
    });

    // Robot runs continuously
    while (true) {
        robot.moveForward();
    }

    camThread.join();  // Wait for the camera thread to finish (normally never reached)

    return 0;
}
