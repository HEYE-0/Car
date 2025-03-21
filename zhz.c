#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <wiringPi.h>
#include <softPwm.h>

#define Trig  28
#define Echo  29
#define LEFT  27
#define RIGHT 26
#define BUFSIZE 512

#define MOTOR_GO_FORWARD   digitalWrite(1,HIGH); digitalWrite(4,LOW); digitalWrite(5,HIGH); digitalWrite(6,LOW)
#define MOTOR_GO_BACK      digitalWrite(4,HIGH); digitalWrite(1,LOW); digitalWrite(6,HIGH); digitalWrite(5,LOW)
#define MOTOR_GO_RIGHT     digitalWrite(1,HIGH); digitalWrite(4,LOW); digitalWrite(6,HIGH); digitalWrite(5,LOW)
#define MOTOR_GO_LEFT      digitalWrite(4,HIGH); digitalWrite(1,LOW); digitalWrite(5,HIGH); digitalWrite(6,LOW)
#define MOTOR_GO_STOP      digitalWrite(1, LOW); digitalWrite(4,LOW); digitalWrite(5, LOW); digitalWrite(6,LOW)

// GPIO 初始化
void setup() {
    if (wiringPiSetup() == -1) {
        fprintf(stderr, "Setup failed!\n");
        exit(1);
    }
    
    pinMode(1, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);
    pinMode(LEFT, INPUT);   // 左传感器
    pinMode(RIGHT, INPUT);  // 右传感器

    softPwmCreate(1, 0, 500);
    softPwmCreate(4, 0, 500);
    softPwmCreate(5, 0, 500);
    softPwmCreate(6, 0, 500);
}

// 前进
void run() {
    printf("[INFO] Moving Forward\n");
    softPwmWrite(1, 250);
    softPwmWrite(4, 0);
    softPwmWrite(5, 250);
    softPwmWrite(6, 0);
}

// 停止
void brake() {
    printf("[INFO] Stopping\n");
    softPwmWrite(1, 0);
    softPwmWrite(4, 0);
    softPwmWrite(5, 0);
    softPwmWrite(6, 0);
}

// 左转
void left() {
    printf("[INFO] Turning Left\n");
    softPwmWrite(4, 250);
    softPwmWrite(1, 0);
    softPwmWrite(6, 0);
    softPwmWrite(5, 250);
}

// 右转
void right() {
    printf("[INFO] Turning Right\n");
    softPwmWrite(4, 0);
    softPwmWrite(1, 250);
    softPwmWrite(6, 250);
    softPwmWrite(5, 0);
}

// 后退
void back() {
    printf("[INFO] Moving Backward\n");
    softPwmWrite(1, 250);
    softPwmWrite(4, 0);
    softPwmWrite(5, 250);
    softPwmWrite(6, 0);
}

int main() {
    setup();  // 初始化 GPIO

    int SR, SL;

    while (1) {
        SR = digitalRead(RIGHT);
        SL = digitalRead(LEFT);

        if (SL == LOW && SR == LOW) {   // 两边均无障碍，前进
            run();
        } 
        else if (SL == HIGH && SR == LOW) {  // 左侧检测到障碍物，右转
            right();
        } 
        else if (SR == HIGH && SL == LOW) {  // 右侧检测到障碍物，左转
            left();
        } 
        else {  // 两侧均检测到障碍物，停止
            brake();
        }

        delay(50);  // 避免 CPU 过载
    }

    return 0;
}


