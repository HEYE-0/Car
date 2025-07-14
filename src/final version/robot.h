#ifndef ROBOT_H
#define ROBOT_H

#include "motor.h"
#include "ultrasonic.h"
#include "camera.h"
#include <vector>

class Robot {
public:
    Robot();
    ~Robot();

    void moveForward(int speed = 50);
    void turnLeft(int speed = 50);
    void turnRight(int speed = 50);
    void stopAll();

    Ultrasonic* getUltrasonic(int index);
    Camera* getCamera();

private:
    Motor leftMotor;
    Motor rightMotor;
    std::vector<Ultrasonic*> sensors; // front, left, right
    Camera camera;
};

#endif
