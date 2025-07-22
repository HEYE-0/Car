#include <stdio.h>  
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <wiringPi.h>       // Include the wiringPi library for GPIO control
#include <softPwm.h>        // Include the softPWM library for simulating PWM signals

// Define GPIO pins
#define Trig  28  // GPIO pin connected to the Trig pin of the ultrasonic sensor
#define Echo  29  // GPIO pin connected to the Echo pin of the ultrasonic sensor
#define LEFT  27  // GPIO pin connected to the left obstacle sensor
#define RIGHT 26  // GPIO pin connected to the right obstacle sensor
#define BUFSIZE 512 // Buffer size

// Define motor control logic
#define MOTOR_GO_FORWARD   digitalWrite(1, HIGH); digitalWrite(4, LOW); digitalWrite(5, HIGH); digitalWrite(6, LOW)
#define MOTOR_GO_BACK      digitalWrite(4, HIGH); digitalWrite(1, LOW); digitalWrite(6, HIGH); digitalWrite(5, LOW)
#define MOTOR_GO_RIGHT     digitalWrite(1, HIGH); digitalWrite(4, LOW); digitalWrite(6, HIGH); digitalWrite(5, LOW)
#define MOTOR_GO_LEFT      digitalWrite(4, HIGH); digitalWrite(1, LOW); digitalWrite(5, HIGH); digitalWrite(6, LOW)
#define MOTOR_GO_STOP      digitalWrite(1, LOW); digitalWrite(4, LOW); digitalWrite(5, LOW); digitalWrite(6, LOW)

// GPIO initialization
void setup() {
    if (wiringPiSetup() == -1) {  // Initialize the wiringPi library, exit if failed
        fprintf(stderr, "Setup failed!\n");
        exit(1);
    }
    
    // Set GPIO pin modes
    pinMode(1, OUTPUT);  // Set pin 1 as output to control the motor
    pinMode(4, OUTPUT);  // Set pin 4 as output to control the motor
    pinMode(5, OUTPUT);  // Set pin 5 as output to control the motor
    pinMode(6, OUTPUT);  // Set pin 6 as output to control the motor
    pinMode(LEFT, INPUT);   // Set the left obstacle sensor pin as input
    pinMode(RIGHT, INPUT);  // Set the right obstacle sensor pin as input

    // Initialize soft PWM signals, with parameters being pin number, initial value, and maximum value
    softPwmCreate(1, 0, 500);  // Initialize soft PWM on pin 1 for motor control
    softPwmCreate(4, 0, 500);  // Initialize soft PWM on pin 4 for motor control
    softPwmCreate(5, 0, 500);  // Initialize soft PWM on pin 5 for motor control
    softPwmCreate(6, 0, 500);  // Initialize soft PWM on pin 6 for motor control
}

// Move forward
void run() {
    printf("[INFO] Moving Forward\n");
    // Set motor control signals to move forward
    softPwmWrite(1, 250);
    softPwmWrite(4, 0);
    softPwmWrite(5, 250);
    softPwmWrite(6, 0);
}

// Stop (brake)
void brake() {
    printf("[INFO] Stopping\n");
    // Set motor control signals to stop
    softPwmWrite(1, 0);
    softPwmWrite(4, 0);
    softPwmWrite(5, 0);
    softPwmWrite(6, 0);
}

// Turn left
void left() {
    printf("[INFO] Turning Left\n");
    // Set motor control signals to turn left
    softPwmWrite(4, 250);
    softPwmWrite(1, 0);
    softPwmWrite(6, 0);
    softPwmWrite(5, 250);
}

// Turn right
void right() {
    printf("[INFO] Turning Right\n");
    // Set motor control signals to turn right
    softPwmWrite(4, 0);
    softPwmWrite(1, 250);
    softPwmWrite(6, 250);
    softPwmWrite(5, 0);
}

// Move backward
void back() {
    printf("[INFO] Moving Backward\n");
    // Set motor control signals to move backward
    softPwmWrite(1, 250);
    softPwmWrite(4, 0);
    softPwmWrite(5, 250);
    softPwmWrite(6, 0);
}

int main() {
    setup();  // Initialize GPIO

    int SR, SL;

    while (1) {
        // Read the status of the right and left obstacle sensors
        SR = digitalRead(RIGHT);  // Read the value of the right sensor
        SL = digitalRead(LEFT);   // Read the value of the left sensor

        // Determine actions based on sensor status
        if (SL == LOW && SR == LOW) {   // No obstacles on both sides, move forward
            run();
        } 
        else if (SL == HIGH && SR == LOW) {  // Left side detects an obstacle, turn right
            right();
        } 
        else if (SR == HIGH && SL == LOW) {  // Right side detects an obstacle, turn left
            left();
        } 
        else {  // Obstacles detected on both sides, stop the car
            brake();
        }

    }

    return 0;
}
