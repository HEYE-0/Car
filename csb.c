#include <stdio.h>
#include <stdlib.h>
#include <softPwm.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/time.h>
#include <wiringPi.h>

#define Trig 28  // 超声波触发引脚
#define Echo 29  // 超声波回响引脚
#define BUFSIZE 512

// 初始化超声波传感器
void ultraInit(void)
{
    pinMode(Echo, INPUT);
    pinMode(Trig, OUTPUT);
}

// 测量距离，返回值单位为 cm
float disMeasure(void)
{
    struct timeval tv1, tv2;
    long start, stop;
    float dis;
    int timeout = 5000;  // 超时限制 5ms，防止死循环
    int count = 0;

    // 发送触发信号
    digitalWrite(Trig, LOW);
    delayMicroseconds(2);
    digitalWrite(Trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(Trig, LOW);

    // 等待回波信号，最多等待 5ms
    while (!(digitalRead(Echo) == 1))
    {
        count++;
        if (count > timeout) return -1; // 超时返回 -1
    }
    gettimeofday(&tv1, NULL);

    count = 0;
    while (!(digitalRead(Echo) == 0))
    {
        count++;
        if (count > timeout) return -1; // 超时返回 -1
    }
    gettimeofday(&tv2, NULL);

    // 计算时间差并转换为距离
    start = tv1.tv_sec * 1000000 + tv1.tv_usec;
    stop = tv2.tv_sec * 1000000 + tv2.tv_usec;
    dis = (float)(stop - start) / 1000000 * 34000 / 2;
    return dis;
}

// 使小车前进，持续时间为 duration 毫秒
void run(int duration)
{
    unsigned long start_time = millis();
    while (millis() - start_time < duration)
    {
        softPwmWrite(1, 0);
        softPwmWrite(4, 500);
        softPwmWrite(5, 0);
        softPwmWrite(6, 500);
    }
}

// 使小车停止
void brake()
{
    softPwmWrite(1, 0);
    softPwmWrite(4, 0);
    softPwmWrite(5, 0);
    softPwmWrite(6, 0);
}

// 使小车左转，持续时间为 duration 毫秒
void left(int duration)
{
    unsigned long start_time = millis();
    while (millis() - start_time < duration)
    {
        softPwmWrite(1, 0);
        softPwmWrite(4, 0);
        softPwmWrite(5, 0);
        softPwmWrite(6, 500);
    }
}

// 使小车右转，持续时间为 duration 毫秒
void right(int duration)
{
    unsigned long start_time = millis();
    while (millis() - start_time < duration)
    {
        softPwmWrite(1, 0);
        softPwmWrite(4, 500);
        softPwmWrite(5, 0);
        softPwmWrite(6, 0);
    }
}

// 使小车后退，持续时间为 duration 毫秒
void back(int duration)
{
    unsigned long start_time = millis();
    while (millis() - start_time < duration)
    {
        softPwmWrite(1, 500);
        softPwmWrite(4, 0);
        softPwmWrite(5, 500);
        softPwmWrite(6, 0);
    }
}

int main()
{
    float dis;

    // 初始化 WiringPi，如果失败则退出
    if (wiringPiSetup() == -1)
    {
        printf("WiringPi 初始化失败！\n");
        return -1;
    }

    // 设置电机控制引脚
    pinMode(1, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);

    // 初始化 PWM，初始值设为 0，避免电机抖动
    softPwmCreate(1, 0, 500);
    softPwmCreate(4, 0, 500);
    softPwmCreate(5, 0, 500);
    softPwmCreate(6, 0, 500);

    while (1)
    {
        // 测量前方障碍物距离
        dis = disMeasure();
        if (dis == -1)
        {
            printf("测距失败！\n");
            continue;
        }

        printf("距离 = %.2f cm\n", dis);

        if (dis > 4 && dis < 30)
        {
            // 过近时左转避障
            left(300);
        }
        else if (dis > 30 && dis < 450)
        {
            // 安全距离时前进
            run(500);
        }
        else
        {
            // 其他情况刹车
            brake();
        }
    }

    return 0;
}
