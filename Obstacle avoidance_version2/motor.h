#ifndef MOTOR_H
#define MOTOR_H

class Motor {
public:
    Motor(int in1, int in2, int pwm);
    void forward(int speed);
    void backward(int speed);
    void stop();

private:
    int _in1, _in2, _pwm;
};

#endif
