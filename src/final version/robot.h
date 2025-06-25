// Updated Robot header (robot.h)
#ifndef ROBOT_H
#define ROBOT_H

#include "motor.h"
#include "ultrasonic.h"
#include <vector>

class Robot {
public:
    Robot();
    virtual ~Robot();

    void moveForward();
    void turnLeft();
    void turnRight();
    void moveForward(int speed);
    void turnLeft(int speed);
    void turnRight(int speed);
    void stopAll();

    float getSensorDistance(int index);

private:
    Motor leftMotor;
    Motor rightMotor;
    std::vector<Ultrasonic*> sensors;
};

#endif // ROBOT_H
