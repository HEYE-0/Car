#include <wiringPi.h>
#include "robot.h"

int main() {
    wiringPiSetup();
    Robot robot;
    while (true) {
        robot.moveForward();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    return 0;
}