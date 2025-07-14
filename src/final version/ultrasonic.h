#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <gpiod.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <string>

// Interface for receiving distance events
class DistanceEventInterface {
public:
    virtual void onTooClose(float distance, int id) = 0;
    virtual ~DistanceEventInterface() = default;
};

class Ultrasonic {
public:
    Ultrasonic(const std::string& chipname, int trig_line, int echo_line, int id);
    ~Ultrasonic();

    void registerCallback(DistanceEventInterface* cb);
    void start();
    void notifyMeasure();
    void stop();

private:
    void monitorLoop();

    enum class State {
        IDLE,
        TRIGGER,
        WAIT_FOR_RISING,
        WAIT_FOR_FALLING
    };

    gpiod_chip* _chip;
    gpiod_line* _trig;
    gpiod_line* _echo;

    std::string _chipname;
    int _trigLine;
    int _echoLine;
    int _id;

    DistanceEventInterface* _callback;
    std::thread _thread;

    std::mutex _mutex;
    std::condition_variable _cv;
    bool _running;
    bool _shouldMeasure;
    State _state;
    std::chrono::steady_clock::time_point _echoStart;
};

#endif // ULTRASONIC_H
