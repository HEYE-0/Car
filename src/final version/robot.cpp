#include "robot.h"
#include <iostream>

Robot::Robot() : leftMotor(1, 2, 3), rightMotor(4, 5, 6) {
    sensors.push_back(new Ultrasonic("gpiochip0", 17, 18, 0)); // Front
    sensors.push_back(new Ultrasonic("gpiochip0", 27, 22, 1)); // Left
    sensors.push_back(new Ultrasonic("gpiochip0", 23, 24, 2)); // Right
}

Robot::~Robot() {
    for (auto* sensor : sensors) {
        delete sensor;
    }
}

void Robot::moveForward() {
    leftMotor.forward(100);
    rightMotor.forward(100);
}

void Robot::turnLeft() {
    leftMotor.backward(100);
    rightMotor.forward(100);
}

void Robot::turnRight() {
    leftMotor.forward(100);
    rightMotor.backward(100);
}

void Robot::moveForward(int speed) {
    leftMotor.forward(speed);
    rightMotor.forward(speed);
}

void Robot::turnLeft(int speed) {
    leftMotor.backward(speed);
    rightMotor.forward(speed);
}

void Robot::turnRight(int speed) {
    leftMotor.forward(speed);
    rightMotor.backward(speed);
}

void Robot::stopAll() {
    leftMotor.stop();
    rightMotor.stop();
}

float Robot::getSensorDistance(int index) {
    if (index >= 0 && static_cast<size_t>(index) < sensors.size()) {
        return sensors[index]->getDistance();
    }
    return -1.0f;
}
