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

// Define GPIO pin numbers for sensor and motor control
#define Trig  28  // Ultrasonic trigger pin
#define Echo  29  // Ultrasonic echo pin
#define LEFT  27  // Left sensor (used for line detection)
#define RIGHT 26  // Right sensor (used for line detection)
#define BUFSIZE 512

// Define motor control macros
#define MOTOR_GO_FORWARD  digitalWrite(1,HIGH); digitalWrite(4,LOW); digitalWrite(5,HIGH); digitalWrite(6,LOW)
#define MOTOR_GO_BACK     digitalWrite(4,HIGH); digitalWrite(1,LOW); digitalWrite(6,HIGH); digitalWrite(5,LOW)
#define MOTOR_GO_RIGHT    digitalWrite(1,HIGH); digitalWrite(4,LOW); digitalWrite(6,HIGH); digitalWrite(5,LOW)
#define MOTOR_GO_LEFT     digitalWrite(4,HIGH); digitalWrite(1,LOW); digitalWrite(5,HIGH); digitalWrite(6,LOW)
#define MOTOR_GO_STOP     digitalWrite(1, LOW); digitalWrite(4,LOW); digitalWrite(5, LOW); digitalWrite(6,LOW)

// Move forward
void run() {
    softPwmWrite(4, 0);   // Turn off the left rear motor
    softPwmWrite(1, 250); // Start the left front motor
    softPwmWrite(6, 0);   // Turn off the right rear motor
    softPwmWrite(5, 250); // Start the right front motor
}

// Brake (Stop)
void brake() {
    softPwmWrite(1, 0); // Turn off the left front motor
    softPwmWrite(4, 0); // Turn off the left rear motor
    softPwmWrite(5, 0); // Turn off the right front motor
    softPwmWrite(6, 0); // Turn off the right rear motor
}

// Turn left
void left() {
    softPwmWrite(4, 250); // Start the left rear motor
    softPwmWrite(1, 0);   // Turn off the left front motor
    softPwmWrite(6, 0);   // Turn off the right rear motor
    softPwmWrite(5, 250); // Start the right front motor
}

// Control the car to turn
void turn(int time) {
    left(); // Call the left turn function
    delay(time * 200); // Control the turning angle by time
}

// Turn right
void right() {
    softPwmWrite(4, 0);   // Turn off the left rear motor
    softPwmWrite(1, 250); // Start the left front motor
    softPwmWrite(6, 250); // Start the right rear motor
    softPwmWrite(5, 0);   // Turn off the right front motor
}

// Move backward
void back(int time) {
    softPwmWrite(4, 250); // Start the left rear motor
    softPwmWrite(1, 0);   // Turn off the left front motor
    softPwmWrite(6, 250); // Start the right rear motor
    softPwmWrite(5, 0);   // Turn off the right front motor
    delay(time * 100); // Control the backward time
}

int main(int argc, char *argv[]) {
    int SR, SL;
    
    // Initialize WiringPi
    if (wiringPiSetup() == -1) {
        fprintf(stderr, "WiringPi initialization failed!\n");
        return 1;
    }
    
    // Initialize motor control pins
    pinMode(1, OUTPUT); // IN1
    pinMode(4, OUTPUT); // IN2
    pinMode(5, OUTPUT); // IN3
    pinMode(6, OUTPUT); // IN4
    pinMode(LEFT, INPUT);  // Set left sensor as input (for line detection)
    pinMode(RIGHT, INPUT); // Set right sensor as input (for line detection)
    
    // Initialize PWM control
    softPwmCreate(1, 1, 500);   
    softPwmCreate(4, 1, 500);
    softPwmCreate(5, 1, 500);
    softPwmCreate(6, 1, 500);
    
    while(1) {
        // Read sensor status
        SR = digitalRead(RIGHT); // Read the right sensor data (whether it detects the black line)
        SL = digitalRead(LEFT);  // Read the left sensor data (whether it detects the black line)
        
        if (SL == HIGH && SR == HIGH) { // Both sensors detect the black line, move forward
            printf("GO\n");
            run();
        } else if (SL == HIGH && SR == LOW) { // Right sensor deviates from the black line, correct by turning left
            printf("RIGHT\n");
            left();
        } else if (SR == HIGH && SL == LOW) { // Left sensor deviates from the black line, correct by turning right
            printf("LEFT\n");
            right();
        } else if (SR == LOW && SL == LOW) { // Both sensors do not detect the black line, move backward and adjust direction
            printf("BACK\n");
            back(1);
            turn(2);
        } else { // In other cases, stop
            printf("STOP\n");
            brake();
        }
    }
    
    return 0;
}
