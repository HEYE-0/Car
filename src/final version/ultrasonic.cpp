#include "ultrasonic.h"
#include <iostream>

Ultrasonic::Ultrasonic(const std::string& chipname, int trig_line, int echo_line, int id)
    : _chipname(chipname), _trigLine(trig_line), _echoLine(echo_line), _id(id),
      _callback(nullptr), _running(true), _shouldMeasure(false), _state(State::IDLE) {
    _chip = gpiod_chip_open_by_name(_chipname.c_str());
    _trig = gpiod_chip_get_line(_chip, _trigLine);
    _echo = gpiod_chip_get_line(_chip, _echoLine);

    gpiod_line_request_output(_trig, "ultrasonic_trig", 0);
    gpiod_line_request_both_edges_events(_echo, "ultrasonic_echo");
}

Ultrasonic::~Ultrasonic() {
    stop();
    gpiod_line_release(_trig);
    gpiod_line_release(_echo);
    gpiod_chip_close(_chip);
}

void Ultrasonic::registerCallback(DistanceEventInterface* cb) {
    _callback = cb;
}

void Ultrasonic::start() {
    _thread = std::thread(&Ultrasonic::monitorLoop, this);
}

void Ultrasonic::notifyMeasure() {
    {
        std::lock_guard<std::mutex> lock(_mutex);
        if (_state == State::IDLE) {
            _state = State::TRIGGER;
        }
    }
    _cv.notify_one();
}

void Ultrasonic::stop() {
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _running = false;
        _shouldMeasure = true;  // 触发唤醒以便退出
        _cv.notify_one();
    }
    if (_thread.joinable()) {
        _thread.join();
    }
}

void Ultrasonic::monitorLoop() {
    using namespace std::chrono;
    timespec timeout = {0, 100000000}; // 100ms

    while (_running) {
        std::unique_lock<std::mutex> lock(_mutex);
        _cv.wait(lock, [this]() { return _state != State::IDLE || !_running; });
        if (!_running) break;

        switch (_state) {
            case State::TRIGGER:
                gpiod_line_set_value(_trig, 0);
                for (volatile int i = 0; i < 50; ++i); // 短busy wait替代usleep
                gpiod_line_set_value(_trig, 1);
                for (volatile int i = 0; i < 200; ++i);
                gpiod_line_set_value(_trig, 0);
                _state = State::WAIT_FOR_RISING;
                break;

            case State::WAIT_FOR_RISING:
                lock.unlock();
                if (gpiod_line_event_wait(_echo, &timeout) == 1) {
                    gpiod_line_event event;
                    gpiod_line_event_read(_echo, &event);
                    if (event.event_type == GPIOD_LINE_EVENT_RISING_EDGE) {
                        _echoStart = steady_clock::now();
                        lock.lock();
                        _state = State::WAIT_FOR_FALLING;
                        break;
                    }
                }
                lock.lock();
                _state = State::IDLE;
                break;

            case State::WAIT_FOR_FALLING:
                lock.unlock();
                if (gpiod_line_event_wait(_echo, &timeout) == 1) {
                    gpiod_line_event event;
                    gpiod_line_event_read(_echo, &event);
                    if (event.event_type == GPIOD_LINE_EVENT_FALLING_EDGE) {
                        auto echo_end = steady_clock::now();
                        auto duration_us = duration_cast<microseconds>(echo_end - _echoStart).count();
                        float distance = duration_us * 0.0343f / 2.0f;
                        if (_callback && distance > 0 && distance < 20.0f) {
                            _callback->onTooClose(distance, _id);
                        }
                    }
                }
                lock.lock();
                _state = State::IDLE;
                break;

            default:
                _state = State::IDLE;
                break;
        }
    }
}
