#include <wiringPi.h>
#include "robot.h"

int main() {
    wiringPiSetup();
    Robot robot;
    while (true) {
        robot.moveForward();
        
    }
    return 0;
}