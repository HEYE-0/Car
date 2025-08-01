#include "robot.h"
#include <thread>
#include <chrono>

Robot::Robot() {
    motors[0] = new Motor(0, 1, 26);   // Front left
    motors[1] = new Motor(2, 3, 23);   // Rear left
    motors[2] = new Motor(21, 22, 28); // Front right
    motors[3] = new Motor(24, 25, 29); // Rear right

    sensors[0] = new Ultrasonic(15, 16, 0); // Front
    sensors[1] = new Ultrasonic(4, 5, 1);   // Left
    sensors[2] = new Ultrasonic(6, 7, 2);   // Right

    for (int i = 0; i < 3; ++i) {
        sensors[i]->registerCallback(this);
        sensors[i]->start();
    }
}

Robot::~Robot() {
    for (int i = 0; i < 4; ++i) delete motors[i];
    for (int i = 0; i < 3; ++i) delete sensors[i];
}

void Robot::moveForward() {
    for (int i = 0; i < 4; ++i) motors[i]->forward(512);
}

void Robot::turnLeft() {
    motors[0]->backward(512);
    motors[1]->backward(512);
    motors[2]->forward(512);
    motors[3]->forward(512);
}

void Robot::turnRight() {
    motors[0]->forward(512);
    motors[1]->forward(512);
    motors[2]->backward(512);
    motors[3]->backward(512);
}

void Robot::stopAll() {
    for (int i = 0; i < 4; ++i) motors[i]->stop();
}

void Robot::onTooClose(float distance, int sensorId) {
    stopAll();

    if (sensorId == 0) { // Front
        float leftDist = sensors[1]->getDistance();
        float rightDist = sensors[2]->getDistance();
        if (leftDist > rightDist) turnLeft();
        else turnRight();
        moveForward();
    } else if (sensorId == 1) {
        turnRight();
        moveForward();
    } else if (sensorId == 2) {
        turnLeft();
        moveForward();
    }
}
