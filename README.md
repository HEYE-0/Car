# Obstacle Avoidance Robot Project (Qt + C++)

## 📌 Project Overview

This project is developed based on the Raspberry Pi 5 and fully implemented in C++. It builds an autonomous obstacle avoidance robot system. A Qt graphical interface enables real-time control of the robot’s movement, motor speed adjustment, ultrasonic distance monitoring, and camera feed display. The architecture is modular and features a custom multithreaded real-time scheduler (TaskScheduler) to replace traditional polling, achieving soft real-time control.

---

## 📂 Project Folder Overview

### 🔄 UPDATE

Contains version transition documentation. Each file outlines the changes made between specific software versions, providing a clear development history.

#### Files:
- `PREVIOUS TO VERSION1.txt`
- `VERSION1 TO VERSION2.txt`
- `VERSION2 TO VERSION3.txt`
- `VERSION3 TO VERSION4.txt`

### 📄 docs

This folder includes essential system documentation.

#### Files:
- `Structural description.txt`: Overview of the system’s structure.
- `Hardware_Specifications.pdf`: Technical details about the hardware components.
- `System architecture.pdf`: Explanation of the overall system design.
- `Team Roles.md`: Division of team work.
- `background.docx`:Background of this project.
- `Project Revision Meeting Minutes.docx`: Meeting minutes.

### 🎞 media

Stores multimedia assets demonstrating the system.

#### Files:
- `demo.mp4`: Project demonstration video.
- `demostration video link.txt`: Link to an online version of the demonstration.

### 🧠 src

The source code directory. It contains both development history and the final implementation.

#### Subfolders:
- `old versions/`: Legacy code for reference.
- `final version/`: Stable, up-to-date implementation.

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

---

## ⚙️ Build & Run

### 🔧 Dependency Installation (Run Once)

```bash
sudo apt update
sudo apt install -y qtbase5-dev qt5-qmake qttools5-dev-tools libopencv-dev libgpiod-dev build-essential pkg-config
```


---

### 🔧 Build the Project

```bash
make clean
make         # Build the Qt GUI application (robot_gui)
make test    # Build the command-line testing tool (test_robot)
```
### 🚀 Run the Application

#### 🚧 If using SSH (no desktop):

```bash
export QT_QPA_PLATFORM=offscreen
./robot_gui
```

#### 🧪 Run the test robot program:

```bash
sudo ./test_robot
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
- Entirely written in C++

---

## 📷 Screenshots / Demo

[(https://t.bilibili.com/1058982800262168584?share_source=pc_native
https://youtu.be/4GdFb-aglIA)]
- Preview of the Qt control panel
- Hardware wiring diagram (motors, sensors, camera)

---

## 👨‍💻 Team Members

- Ye He (2966900H)
- Yuan Zhao (3026815Z)
- Hao Wu (2982755W)
- Xiaoyun Ma (3010324M)
- Zhaojie Zhu (2982461Z)
