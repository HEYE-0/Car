#include "ultrasonic.h"
#include <chrono>
#include <thread>
#include <iostream>

Ultrasonic::Ultrasonic(const std::string& chipname, int trig_line, int echo_line, int id)
    : _chipname(chipname), _trigLine(trig_line), _echoLine(echo_line), _id(id), _callback(nullptr) {
    _chip = gpiod_chip_open_by_name(_chipname.c_str());
    _trig = gpiod_chip_get_line(_chip, _trigLine);
    _echo = gpiod_chip_get_line(_chip, _echoLine);

    gpiod_line_request_output(_trig, "ultrasonic_trig", 0);
    gpiod_line_request_both_edges_events(_echo, "ultrasonic_echo");
}

Ultrasonic::~Ultrasonic() {
    gpiod_line_release(_trig);
    gpiod_line_release(_echo);
    gpiod_chip_close(_chip);
}

void Ultrasonic::registerCallback(DistanceEventInterface* cb) {
    _callback = cb;
}

void Ultrasonic::start() {
    _thread = std::thread(&Ultrasonic::monitorLoop, this);
    _thread.detach();
}

float Ultrasonic::getDistance() {
    using namespace std::chrono;

    gpiod_line_set_value(_trig, 0);
    gpiod_line_set_value(_trig, 1);
    gpiod_line_set_value(_trig, 0);

    gpiod_line_event event;
    auto start_time = steady_clock::now();
    auto timeout = timespec{0, 100000000}; // 100ms

    // Waiting for rising edge
    if (gpiod_line_event_wait(_echo, &timeout) != 1 ||
        gpiod_line_event_read(_echo, &event) != 0 || event.event_type != GPIOD_LINE_EVENT_RISING_EDGE) {
        return -1.0f;  // Timeout or error
    }
    auto echo_start = steady_clock::now();

    // Wait for falling edge
    if (gpiod_line_event_wait(_echo, &timeout) != 1 ||
        gpiod_line_event_read(_echo, &event) != 0 || event.event_type != GPIOD_LINE_EVENT_FALLING_EDGE) {
        return -1.0f;
    }
    auto echo_end = steady_clock::now();

    auto duration_us = duration_cast<microseconds>(echo_end - echo_start).count();
    return duration_us * 0.0343f / 2.0f; // cm
}

void Ultrasonic::monitorLoop() {
    while (true) {
        float d = getDistance();
        if (_callback && d > 0 && d < 20.0f) {
            _callback->onTooClose(d, _id);
        }
    }
}
