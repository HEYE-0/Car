#include "robot.h"

Robot::Robot()
    : leftMotor(20, 21, 16),
      rightMotor(19, 26, 13),
      camera()
{
    sensors.push_back(new Ultrasonic("gpiochip0", 23, 24, 0)); // front
    sensors.push_back(new Ultrasonic("gpiochip0", 17, 27, 1)); // left
    sensors.push_back(new Ultrasonic("gpiochip0", 5, 6, 2));   // right

    for (auto sensor : sensors) {
        sensor->start();
    }
}

Robot::~Robot() {
    for (auto sensor : sensors) {
        delete sensor;
    }
}

void Robot::moveForward(int speed) {
    leftMotor.forward(speed);
    rightMotor.forward(speed);
}

void Robot::turnLeft(int speed) {
    leftMotor.stop();
    rightMotor.forward(speed);
}

void Robot::turnRight(int speed) {
    leftMotor.forward(speed);
    rightMotor.stop();
}

void Robot::stopAll() {
    leftMotor.stop();
    rightMotor.stop();
}

Ultrasonic* Robot::getUltrasonic(int index) {
    return sensors[index];
}

Camera* Robot::getCamera() {
    return &camera;
}
