# Bluetooth-Controlled Obstacle Avoidance Robot Project (Qt + C++)

## 📌 Project Overview

This project is developed based on the Raspberry Pi 5 and fully implemented in C++. It builds a Bluetooth-controlled autonomous obstacle avoidance robot system. A Qt graphical interface enables real-time control of the robot’s movement, motor speed adjustment, ultrasonic distance monitoring, and camera feed display. The architecture is modular and features a custom multithreaded real-time scheduler (TaskScheduler) to replace traditional polling, achieving soft real-time control.

---

## 🧱 Project Structure

| Module | Description |
|--------|-------------|
| `main.cpp` | Entry point of the Qt application |
| `controlpanel.cpp/h` | GUI panel providing control buttons and data display |
| `scheduler.cpp/h` | Real-time task scheduler supporting multiple periodic callbacks |
| `robot.cpp/h` | Robot control logic integrating motor and sensor functions |
| `motor.cpp/h` | Motor control module with PWM speed adjustment |
| `ultrasonic.cpp/h` | Ultrasonic distance measurement module using `chrono` for precision |
| `camera.cpp/h` | Camera capture module using OpenCV |
| `test_robot.cpp` | Console testing utility for motor and sensor validation |
| `Makefile` | Build configuration for all components |

---

## 🛠️ System Dependencies

**Hardware:**

- Raspberry Pi 5
- 4 DC motors + 2 L298N motor drivers
- 3 HC-SR04 ultrasonic sensors
- Kamera V2 (8MP, 1080p) camera module

**Software:**

- Qt 5 (`qtbase5-dev`)
- OpenCV 4 (`libopencv-dev`)
- wiringPi (or any compatible GPIO control library)
- libgpiod (for edge-based GPIO control)
- C++11 or later

Install dependencies on Raspberry Pi:

```bash
sudo apt update
sudo apt install qt5-default libopencv-dev libgpiod-dev
```

---

## ⚙️ Build & Run

### 🔧 Build

```bash
make         # Build the Qt GUI application (robot_gui)
make test    # Build the command-line testing tool (test_robot)
```

### 🚀 Run

```bash
./robot_gui         # Launch the Qt graphical control interface
sudo ./test_robot   # Run the test program (requires root for GPIO access)
```

---

## 🧠 Real-Time Scheduling Design

- All real-time tasks (camera capture and ultrasonic measurement) are registered in `TaskScheduler` and executed on separate threads.
- Each task runs on a fixed interval (e.g., camera every 50ms, ultrasonic every 300ms).
- `std::condition_variable::wait_until()` is used to control wake-up timing and avoid polling overhead.
- Task execution durations are recorded via `std::chrono` for basic delay analysis.

---

## 🖼️ GUI Features

- **Directional control buttons**: Forward, Backward, Left, Right
- **Speed control slider**: Adjusts motor PWM output (0–1023)
- **Live camera view**: Real-time preview from the front camera
- **Ultrasonic distance display**: Shows distances from front, left, and right sensors
- **Status label**: Displays the robot’s current action (e.g., Forward, Stopped)
- **Start/Stop buttons**: Initializes or shuts down the control system

---

## ✅ Project Highlights

- Fully modular and object-oriented design
- Non-blocking, thread-safe real-time task scheduling
- High-precision ultrasonic measurement using `chrono`
- Decoupled GUI and control logic via Qt + OpenCV
- Entirely written in C++ (no Python, no Arduino)

---

## 📷 Screenshots / Demo

> (Insert project screenshots or video links here)

- Preview of the Qt control panel
- Hardware wiring diagram (motors, sensors, camera)

---

## 👨‍💻 Team Members

- Ye He (2966900H)
- Yuan Zhao (3026815Z)
- Hao Wu (2982755W)
- Xiaoyun Ma (3010324M)
- Zhaojie Zhu (2982461Z)
