#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <wiringPi.h>

// 定义传感器和电机控制的 GPIO 引脚编号
#define Trig  28  // 超声波触发引脚
#define Echo  29  // 超声波回响引脚
#define LEFT  27  // 左侧传感器（用于检测黑线）
#define RIGHT 26  // 右侧传感器（用于检测黑线）
#define BUFSIZE 512

// 定义电机控制宏
#define MOTOR_GO_FORWARD  digitalWrite(1,HIGH); digitalWrite(4,LOW); digitalWrite(5,HIGH); digitalWrite(6,LOW)
#define MOTOR_GO_BACK     digitalWrite(4,HIGH); digitalWrite(1,LOW); digitalWrite(6,HIGH); digitalWrite(5,LOW)
#define MOTOR_GO_RIGHT    digitalWrite(1,HIGH); digitalWrite(4,LOW); digitalWrite(6,HIGH); digitalWrite(5,LOW)
#define MOTOR_GO_LEFT     digitalWrite(4,HIGH); digitalWrite(1,LOW); digitalWrite(5,HIGH); digitalWrite(6,LOW)
#define MOTOR_GO_STOP     digitalWrite(1, LOW); digitalWrite(4,LOW); digitalWrite(5, LOW); digitalWrite(6,LOW)

// 前进
void run() {
    softPwmWrite(4, 0);   // 关闭左后轮电机
    softPwmWrite(1, 250); // 启动左前轮电机
    softPwmWrite(6, 0);   // 关闭右后轮电机
    softPwmWrite(5, 250); // 启动右前轮电机
}

// 刹车（停止）
void brake() {
    softPwmWrite(1, 0); // 关闭左前轮电机
    softPwmWrite(4, 0); // 关闭左后轮电机
    softPwmWrite(5, 0); // 关闭右前轮电机
    softPwmWrite(6, 0); // 关闭右后轮电机
}

// 左转
void left() {
    softPwmWrite(4, 250); // 启动左后轮电机
    softPwmWrite(1, 0);   // 关闭左前轮电机
    softPwmWrite(6, 0);   // 关闭右后轮电机
    softPwmWrite(5, 250); // 启动右前轮电机
}

// 控制小车转弯
void turn(int time) {
    left(); // 调用左转函数
    delay(time * 200); // 通过时间控制转弯角度
}

// 右转
void right() {
    softPwmWrite(4, 0);   // 关闭左后轮电机
    softPwmWrite(1, 250); // 启动左前轮电机
    softPwmWrite(6, 250); // 启动右后轮电机
    softPwmWrite(5, 0);   // 关闭右前轮电机
}

// 后退
void back(int time) {
    softPwmWrite(4, 250); // 启动左后轮电机
    softPwmWrite(1, 0);   // 关闭左前轮电机
    softPwmWrite(6, 250); // 启动右后轮电机
    softPwmWrite(5, 0);   // 关闭右前轮电机
    delay(time * 100); // 控制后退时间
}

int main(int argc, char *argv[]) {
    int SR, SL;
    
    // 初始化 WiringPi
    if (wiringPiSetup() == -1) {
        fprintf(stderr, "WiringPi 初始化失败！\n");
        return 1;
    }
    
    // 初始化电机控制引脚
    pinMode(1, OUTPUT); // IN1
    pinMode(4, OUTPUT); // IN2
    pinMode(5, OUTPUT); // IN3
    pinMode(6, OUTPUT); // IN4
    pinMode(LEFT, INPUT);  // 设置左传感器为输入模式（用于检测黑线）
    pinMode(RIGHT, INPUT); // 设置右传感器为输入模式（用于检测黑线）
    
    // 初始化 PWM 控制
    softPwmCreate(1, 1, 500);   
    softPwmCreate(4, 1, 500);
    softPwmCreate(5, 1, 500);
    softPwmCreate(6, 1, 500);
    
    while(1) {
        // 读取传感器状态
        SR = digitalRead(RIGHT); // 读取右侧传感器数据（是否检测到黑线）
        SL = digitalRead(LEFT);  // 读取左侧传感器数据（是否检测到黑线）
        
        if (SL == HIGH && SR == HIGH) { // 两侧传感器都检测到黑线，前进
            printf("GO\n");
            run();
        } else if (SL == HIGH && SR == LOW) { // 右侧偏离黑线，向左修正
            printf("RIGHT\n");
            left();
        } else if (SR == HIGH && SL == LOW) { // 左侧偏离黑线，向右修正
            printf("LEFT\n");
            right();
        } else if (SR == LOW && SL == LOW) { // 两侧都未检测到黑线，后退并调整方向
            printf("BACK\n");
            back(1);
            turn(2);
        } else { // 其他情况，停止
            printf("STOP\n");
            brake();
        }
    }
    
    return 0;
}


