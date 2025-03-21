#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <wiringPi.h>       // 引入wiringPi库，用于GPIO控制
#include <softPwm.h>        // 引入软PWM库，用于模拟PWM信号

// 定义GPIO引脚
#define Trig  28  // 超声波传感器Trig脚连接的GPIO引脚
#define Echo  29  // 超声波传感器Echo脚连接的GPIO引脚
#define LEFT  27  // 左侧障碍物传感器连接的GPIO引脚
#define RIGHT 26  // 右侧障碍物传感器连接的GPIO引脚
#define BUFSIZE 512 // 缓冲区大小

// 定义电机控制逻辑
#define MOTOR_GO_FORWARD   digitalWrite(1, HIGH); digitalWrite(4, LOW); digitalWrite(5, HIGH); digitalWrite(6, LOW)
#define MOTOR_GO_BACK      digitalWrite(4, HIGH); digitalWrite(1, LOW); digitalWrite(6, HIGH); digitalWrite(5, LOW)
#define MOTOR_GO_RIGHT     digitalWrite(1, HIGH); digitalWrite(4, LOW); digitalWrite(6, HIGH); digitalWrite(5, LOW)
#define MOTOR_GO_LEFT      digitalWrite(4, HIGH); digitalWrite(1, LOW); digitalWrite(5, HIGH); digitalWrite(6, LOW)
#define MOTOR_GO_STOP      digitalWrite(1, LOW); digitalWrite(4, LOW); digitalWrite(5, LOW); digitalWrite(6, LOW)

// GPIO 初始化
void setup() {
    if (wiringPiSetup() == -1) {  // 初始化wiringPi库，如果失败则输出错误信息并退出
        fprintf(stderr, "Setup failed!\n");
        exit(1);
    }
    
    // 设置GPIO引脚模式
    pinMode(1, OUTPUT);  // 设置引脚1为输出，控制电机
    pinMode(4, OUTPUT);  // 设置引脚4为输出，控制电机
    pinMode(5, OUTPUT);  // 设置引脚5为输出，控制电机
    pinMode(6, OUTPUT);  // 设置引脚6为输出，控制电机
    pinMode(LEFT, INPUT);   // 设置左侧障碍物传感器引脚为输入
    pinMode(RIGHT, INPUT);  // 设置右侧障碍物传感器引脚为输入

    // 初始化软PWM信号，参数分别为引脚号、初始值、最大值
    softPwmCreate(1, 0, 500);  // 初始化电机控制引脚1的软PWM
    softPwmCreate(4, 0, 500);  // 初始化电机控制引脚4的软PWM
    softPwmCreate(5, 0, 500);  // 初始化电机控制引脚5的软PWM
    softPwmCreate(6, 0, 500);  // 初始化电机控制引脚6的软PWM
}

// 前进
void run() {
    printf("[INFO] Moving Forward\n");
    // 设置前进的电机控制信号
    softPwmWrite(1, 250);
    softPwmWrite(4, 0);
    softPwmWrite(5, 250);
    softPwmWrite(6, 0);
}

// 停止
void brake() {
    printf("[INFO] Stopping\n");
    // 设置停止的电机控制信号
    softPwmWrite(1, 0);
    softPwmWrite(4, 0);
    softPwmWrite(5, 0);
    softPwmWrite(6, 0);
}

// 左转
void left() {
    printf("[INFO] Turning Left\n");
    // 设置左转的电机控制信号
    softPwmWrite(4, 250);
    softPwmWrite(1, 0);
    softPwmWrite(6, 0);
    softPwmWrite(5, 250);
}

// 右转
void right() {
    printf("[INFO] Turning Right\n");
    // 设置右转的电机控制信号
    softPwmWrite(4, 0);
    softPwmWrite(1, 250);
    softPwmWrite(6, 250);
    softPwmWrite(5, 0);
}

// 后退
void back() {
    printf("[INFO] Moving Backward\n");
    // 设置后退的电机控制信号
    softPwmWrite(1, 250);
    softPwmWrite(4, 0);
    softPwmWrite(5, 250);
    softPwmWrite(6, 0);
}

int main() {
    setup();  // 初始化GPIO

    int SR, SL;

    while (1) {
        // 读取右侧和左侧障碍物传感器的状态
        SR = digitalRead(RIGHT);  // 读取右侧传感器的值
        SL = digitalRead(LEFT);   // 读取左侧传感器的值

        // 判断两侧传感器的状态
        if (SL == LOW && SR == LOW) {   // 两边均无障碍物，前进
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

        delay(50);  // 延迟50ms，避免CPU过载
    }

    return 0;
}


