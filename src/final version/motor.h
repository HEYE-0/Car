#ifndef MOTOR_H
#define MOTOR_H

#include <gpiod.h>
#include <string>

class Motor {
public:
    Motor(int in1_line, int in2_line, int pwm_line);
    ~Motor();

    void forward(int speed);
    void backward(int speed);
    void stop();

private:
    gpiod_chip *chip;
    gpiod_line *in1;
    gpiod_line *in2;
    gpiod_line *pwm;

    void setLine(gpiod_line* line, int value);
};

#endif // MOTOR_H
