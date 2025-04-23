#include <stdio.h> 
#include <stdlib.h>
#include <softPwm.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/time.h>
#include <wiringPi.h>

#define Trig 28  // The trigger pin of the ultrasonic sensor
#define Echo 29  // The echo pin of the ultrasonic sensor
#define BUFSIZE 512

// Initialize the ultrasonic sensor.
void ultraInit(void)
{
    pinMode(Echo, INPUT);  // Set the echo pin to input mode.
    pinMode(Trig, OUTPUT); // Set the trigger pin to output mode.
}

// Measure distance, return value in cm.
float disMeasure(void)
{
    struct timeval tv1, tv2;
    long start, stop;
    float dis;
    int timeout = 5000;  // Timeout limit of 5ms to prevent infinite loops
    int count = 0;

    // Send trigger signal
    digitalWrite(Trig, LOW);
    delayMicroseconds(2);
    digitalWrite(Trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(Trig, LOW);

    // Wait for echo signal, maximum wait time of 5ms
    while (!(digitalRead(Echo) == 1))
    {
        count++;
        if (count > timeout) return -1; // Timeout, return -1
    }
    gettimeofday(&tv1, NULL);

    count = 0;
    while (!(digitalRead(Echo) == 0))
    {
        count++;
        if (count > timeout) return -1; // Timeout, return -1
    }
    gettimeofday(&tv2, NULL);

    // Calculate time difference and convert to distance
    start = tv1.tv_sec * 1000000 + tv1.tv_usec;
    stop = tv2.tv_sec * 1000000 + tv2.tv_usec;
    dis = (float)(stop - start) / 1000000 * 34000 / 2;
    return dis;
}

// Robot movement control function
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

void brake()
{
    softPwmWrite(1, 0);
    softPwmWrite(4, 0);
    softPwmWrite(5, 0);
    softPwmWrite(6, 0);
}

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

    // Initialize WiringPi, exit if failure
    if (wiringPiSetup() == -1)
    {
        printf("WiringPi initialization failed!\n");
        return -1;
    }

    // Set motor control pins
    pinMode(1, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);

    // Initialize PWM with initial value of 0 to prevent motor jitter
    softPwmCreate(1, 0, 500);
    softPwmCreate(4, 0, 500);
    softPwmCreate(5, 0, 500);
    softPwmCreate(6, 0, 500);

    ultraInit(); // Initialize the ultrasonic sensor

    while (1)
    {
        // Measure distance to the obstacle ahead
        dis = disMeasure();
        if (dis == -1)
        {
            printf("Distance measurement failed!\n");
            continue;
        }

        printf("Distance = %.2f cm\n", dis);

        if (dis > 4 && dis < 30)
        {
            // Turn left if the distance is too short
            left(300);
        }
        else if (dis > 30 && dis < 450)
        {
            // Move forward if the distance is safe
            run(500);
        }
        else
        {
            // Brake in other cases
            brake();
        }
    }

    return 0;
}
