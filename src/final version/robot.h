#ifndef ROBOT_H
#define ROBOT_H

#include "motor.h"
#include "ultrasonic.h"

class Robot : public DistanceEventInterface {
public:
    Robot();
    ~Robot();
    void onTooClose(float distance, int sensorId) override;
    void stopAll();

private:
    Motor* motors[4];
    Ultrasonic* sensors[3];

    void moveForward(int speed);
    void turnLeft(int speed);
    void turnRight(int speed);

};

#endif
