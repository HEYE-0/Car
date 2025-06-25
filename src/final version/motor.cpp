// Updated motor.cpp using libgpiod
#include "motor.h"
#include <unistd.h>
#include <iostream>

Motor::Motor(int in1_line, int in2_line, int pwm_line) {
    chip = gpiod_chip_open_by_name("gpiochip0");
    if (!chip) {
        std::cerr << "[Motor] Failed to open gpiochip0" << std::endl;
        return;
    }

    in1 = gpiod_chip_get_line(chip, in1_line);
    in2 = gpiod_chip_get_line(chip, in2_line);
    pwm = gpiod_chip_get_line(chip, pwm_line);

    gpiod_line_request_output(in1, "motor", 0);
    gpiod_line_request_output(in2, "motor", 0);
    gpiod_line_request_output(pwm, "motor", 0);
}

Motor::~Motor() {
    gpiod_line_release(in1);
    gpiod_line_release(in2);
    gpiod_line_release(pwm);
    gpiod_chip_close(chip);
}

void Motor::setLine(gpiod_line* line, int value) {
    if (line) gpiod_line_set_value(line, value);
}

void Motor::forward(int speed) {
    setLine(in1, 1);
    setLine(in2, 0);
    setLine(pwm, speed > 0 ? 1 : 0);
}

void Motor::backward(int speed) {
    setLine(in1, 0);
    setLine(in2, 1);
    setLine(pwm, speed > 0 ? 1 : 0);
}

void Motor::stop() {
    setLine(in1, 0);
    setLine(in2, 0);
    setLine(pwm, 0);
}
