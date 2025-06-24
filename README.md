# Bluetooth-Controlled Obstacle Avoidance Robot (Qt + C++)

## 📌 Project Overview

This project implements a Bluetooth-controlled autonomous obstacle avoidance robot using a Raspberry Pi 5 and written entirely in C++. A Qt-based graphical interface allows users to interact with the robot in real-time, control its movement, monitor sensor data, and view the live camera feed. The system is structured around a modular architecture and features a custom-built multithreaded real-time task scheduler.

---

## 🧱 Project Structure

| Module | Description |
|--------|-------------|
| `main.cpp` | Qt application entry point |
| `controlpanel.cpp/h` | Main GUI window with all user interface elements |
| `scheduler.cpp/h` | Real-time multithreaded task scheduler (replaces polling and timers) |
| `robot.cpp/h` | Integrates motors and ultrasonic sensors with high-level robot control |
| `motor.cpp/h` | Controls individual motors with PWM speed control |
| `ultrasonic.cpp/h` | Handles distance measurement using HC-SR04 sensors |
| `camera.cpp/h` | Captures and delivers frames using OpenCV |
| `test_robot.cpp` | CLI-based motor and sensor testing utility |
| `Makefile` | Compiles both GUI and CLI programs with all necessary dependencies |

---

## 🛠️ Dependencies

**Hardware:**

- Raspberry Pi 5
- 4 DC motors + 2 L298N motor drivers
- 3 × HC-SR04 ultrasonic sensors
- Kamera V2 (8MP, 1080p) camera module

**Software:**

- Qt 5 (`qtbase5-dev`)
- OpenCV 4 (`libopencv-dev`)
- `wiringPi` (or equivalent GPIO control library)
- `libgpiod` for edge-based GPIO
- C++11 standard or later

Install dependencies (on Raspberry Pi):

```bash
sudo apt update
sudo apt install qt5-default libopencv-dev libgpiod-dev
