#include <wiringPi.h>
#include <iostream>
#include <thread>
#include <chrono>
#include "robot.h"

void delayMs(int ms) {
}

int main() {
    wiringPiSetup();

    Robot robot;

    std::cout << "[Test] Start testing robot motors..." << std::endl;

    robot.moveForward();
    std::cout << "[Test] Moving forward..." << std::endl;
    delayMs(1000);

    robot.turnLeft();
    std::cout << "[Test] Turning left..." << std::endl;
    delayMs(1000);

    robot.turnRight();
    std::cout << "[Test] Turning right..." << std::endl;
    delayMs(1000);

    robot.stopAll();
    std::cout << "[Test] Stopped." << std::endl;
    delayMs(1000);

    std::cout << "\n[Test] Reading ultrasonic distances..." << std::endl;
    for (int i = 0; i < 3; ++i) {
        float dist = robot.getSensorDistance(i);
        std::cout << "  Sensor " << i << ": " << dist << " cm" << std::endl;
    }

    std::cout << "[Test] Test completed." << std::endl;
    return 0;
}