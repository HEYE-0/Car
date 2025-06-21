#include <wiringPi.h>
#include "motor.h"

Motor::Motor(int in1, int in2, int pwm) : _in1(in1), _in2(in2), _pwm(pwm) {
    pinMode(_in1, OUTPUT);
    pinMode(_in2, OUTPUT);
    pinMode(_pwm, PWM_OUTPUT);
}

void Motor::forward(int speed) {
    digitalWrite(_in1, HIGH);
    digitalWrite(_in2, LOW);
    pwmWrite(_pwm, speed);
}

void Motor::backward(int speed) {
    digitalWrite(_in1, LOW);
    digitalWrite(_in2, HIGH);
    pwmWrite(_pwm, speed);
}

void Motor::stop() {
    pwmWrite(_pwm, 0);
}
