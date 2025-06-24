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
```

---

## ⚙️ Build & Run

### 🔧 Compile

```bash
make         # Compile the main Qt GUI (robot_gui)
make test    # Compile CLI-based test tool (test_robot)
```

### 🚀 Run

```bash
./robot_gui         # Launch Qt GUI
sudo ./test_robot   # Run console test (requires root for GPIO access)
```

---

## 🧠 Real-Time Scheduling Design

- All real-time operations (camera updates and ultrasonic sensor reads) are handled via a custom `TaskScheduler` class.
- Each task is registered with a fixed execution interval (e.g., 50 ms for camera, 300 ms for sensors).
- The scheduler uses `std::condition_variable::wait_until()` to reduce CPU usage and ensure accurate task timing.
- Each task logs its execution duration using `std::chrono`, enabling basic delay analysis and monitoring.

---

## 🖼️ GUI Features

- **Directional control buttons**: Forward, Backward, Left, Right
- **Speed control slider**: Adjust motor PWM output (0–1023)
- **Live camera view**: Real-time feed from front camera (OpenCV)
- **Ultrasonic readings**: Display of 3 sensor distances (front, left, right)
- **Status label**: Shows current robot status (e.g., Moving forward, Stopped)
- **Start / Stop buttons**: Initialize or shut down robot and sensors

---

## ✅ Highlights

- Fully modular object-oriented architecture
- Non-blocking, thread-safe real-time task management
- Accurate distance measurement using `std::chrono`
- Real-time camera feed integration using OpenCV
- Clean, extensible GUI design using Qt widgets
- Code fully written in C++ (no Python or Arduino)

---

## 📷 Screenshots / Demo

> _(Insert screenshots or a link to a short video here)_

- Qt GUI interface preview
- Hardware wiring diagram (motors, sensors, camera)

---

## 👨‍💻 Team Members

- Ye He (2966900H)
- Yuan Zhao (3026815Z)
- Hao Wu (2982755W)
- Xiaoyun Ma (3010324M)
- Zhaojie Zhu (2982461Z)

---

## 📄 License

This project was developed as part of the ENG5220 Embedded Systems coursework at the University of Glasgow. All rights reserved by the authors.
