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

#include <wiringPi.h>  // 树莓派 GPIO 控制库

// 定义超声波传感器和红外传感器的引脚
#define Trig  28  // 触发超声波传感器信号的引脚
#define Echo  29  // 接收超声波回声信号的引脚
#define LEFT  27  // 左侧红外传感器
#define RIGHT 26  // 右侧红外传感器
#define BUFSIZE 512  // 缓冲区大小

// 定义电机控制宏，简化代码
#define MOTOR_GO_FORWARD   digitalWrite(1,HIGH);digitalWrite(4,LOW);digitalWrite(5,HIGH);digitalWrite(6,LOW)
#define MOTOR_GO_BACK      digitalWrite(4,HIGH);digitalWrite(1,LOW);digitalWrite(6,HIGH);digitalWrite(5,LOW)
#define MOTOR_GO_RIGHT     digitalWrite(1,HIGH);digitalWrite(4,LOW);digitalWrite(6,HIGH);digitalWrite(5,LOW)
#define MOTOR_GO_LEFT      digitalWrite(4,HIGH);digitalWrite(1,LOW);digitalWrite(5,HIGH);digitalWrite(6,LOW)
#define MOTOR_GO_STOP      digitalWrite(1, LOW);digitalWrite(4,LOW);digitalWrite(5, LOW);digitalWrite(6,LOW)

// 小车前进
void run() {
    softPwmWrite(4,0); // 左轮前进
    softPwmWrite(1,250); 
    softPwmWrite(6,0); // 右轮前进
    softPwmWrite(5,250); 
}

// 小车刹车（停止）
void brake() {
    softPwmWrite(1,0); // 左轮停止
    softPwmWrite(4,0); 
    softPwmWrite(5,0); // 右轮停止
    softPwmWrite(6,0); 
}

// 左转
void left() {
    softPwmWrite(4,250); // 左轮后退
    softPwmWrite(1,0); 
    softPwmWrite(6,0);  // 右轮前进
    softPwmWrite(5,250); 
}

// 右转
void right() {
    softPwmWrite(4,0);  // 左轮前进
    softPwmWrite(1,250);
    softPwmWrite(6,250); // 右轮后退
    softPwmWrite(5,0); 
}

// 后退
void back() {
    softPwmWrite(1,250); // 左轮后退
    softPwmWrite(4,0); 
    softPwmWrite(5,250); // 右轮后退
    softPwmWrite(6,0); 
}

int main(int argc, char *argv[]) {
    int SR, SL;  // 右侧和左侧红外传感器的读取值
    
    wiringPiSetup(); // 初始化 WiringPi 库
    
    // 设置电机引脚为输出模式
    pinMode(1, OUTPUT); // 左前进
    pinMode(4, OUTPUT); // 左后退
    pinMode(5, OUTPUT); // 右前进
    pinMode(6, OUTPUT); // 右后退
    
    // 初始化 PWM 控制
    softPwmCreate(1, 1, 500);
    softPwmCreate(4, 1, 500);
    softPwmCreate(5, 1, 500);
    softPwmCreate(6, 1, 500);

    while(1) {
        SR = digitalRead(RIGHT); // 读取右侧红外传感器
        SL = digitalRead(LEFT);  // 读取左侧红外传感器
        
        if (SL == HIGH && SR == HIGH) {  // 两个传感器都检测到黑线（前进）
            printf("GO\n");
            run();
        } 
        else if (SL == HIGH && SR == LOW) {  // 右侧偏离黑线，左转纠正
            printf("RIGHT\n");
            left();
        } 
        else if (SR == HIGH && SL == LOW) {  // 左侧偏离黑线，右转纠正
            printf("LEFT\n");
            right();
        } 
        else {  // 两个传感器都没有检测到黑线，停车
            printf("STOP\n");
            brake();
        }
    }
    
    return 0;
}

