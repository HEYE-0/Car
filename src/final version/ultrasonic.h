#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <gpiod.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <string>

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

    /**
     * Safely stop the monitoring thread and clean up.
     */
    void stop();

private:
    /**
     * Internal thread loop implementing the state machine.
     */
    void monitorLoop();

    /**
     * Represents the current state of the state machine.
     */
    enum class State {
        IDLE,
        TRIGGER,
        WAIT_FOR_RISING,
        WAIT_FOR_FALLING
    };

    // GPIO resources
    gpiod_chip* _chip;
    gpiod_line* _trig;
    gpiod_line* _echo;

    // Sensor configuration
    std::string _chipname;
    int _trigLine;
    int _echoLine;
    int _id;

    // Callback interface
    DistanceEventInterface* _callback;

    // Thread and synchronization
    std::thread _thread;
    std::mutex _mutex;
    std::condition_variable _cv;
    bool _running;
    bool _shouldMeasure;
    State _state;
    std::chrono::steady_clock::time_point _echoStart;
};

#endif // ULTRASONIC_H
