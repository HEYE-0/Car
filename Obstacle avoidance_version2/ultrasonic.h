#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <gpiod.h>
#include <thread>
#include <string>

class DistanceEventInterface {
public:
    virtual void onTooClose(float distance, int sensorId) = 0;
};

class Ultrasonic {
public:
    Ultrasonic(const std::string& chipname, int trig_line, int echo_line, int id);
    ~Ultrasonic();
    float getDistance();
    void registerCallback(DistanceEventInterface* cb);
    void start();

private:
    std::string _chipname;
    int _trigLine, _echoLine, _id;
    gpiod_chip* _chip;
    gpiod_line* _trig;
    gpiod_line* _echo;
    DistanceEventInterface* _callback;
    void monitorLoop();
    std::thread _thread;
};

#endif