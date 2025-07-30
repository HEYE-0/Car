# Obstacle Avoidance Robot Project (Qt + C++)

## 1. Project Overview

This project is developed on the Raspberry Pi 5 and fully implemented in C++. It builds an autonomous obstacle-avoidance robot system with modular architecture. A Qt-based graphical interface allows real-time control of the robot’s movement, motor speed adjustment, ultrasonic sensor feedback, and live camera feed display.

The system is designed with a clean, event-driven architecture. A multithreaded control mechanism is used in combination with finite state machines to ensure responsive and organized behavior. This replaces earlier polling or delay-based logic and enables soft real-time responsiveness while maintaining code clarity and scalability.

---

## 2. Project Folder Overview

###  UPDATE

Contains version transition documentation. Each file outlines the changes made between specific software versions, providing a clear development history.

#### Files:
`PREVIOUS TO VERSION1.txt`
`VERSION1 TO VERSION2.txt`
`VERSION2 TO VERSION3.txt`
`VERSION3 TO VERSION4.txt`

###  docs

This folder includes essential system documentation.

#### Files:
- `Structural description.txt`: Overview of the system’s structure.
- `Hardware_Specifications.pdf`: Technical details about the hardware components.
- `System architecture.pdf`: Explanation of the overall system design.
- `Team Roles.md`: Division of team work.
- `background.docx`:Background of this project.
- `Project Revision Meeting Minutes.docx`: Meeting minutes.

###  media

Stores multimedia assets demonstrating the system.

#### Files:
- `demo.mp4`: Project demonstration video.
- `demostration video link.txt`: Link to an online version of the demonstration.

###  src

The source code directory. It contains both development history and the final implementation.

#### Subfolders:
- `old versions/`: Legacy code for reference.
- `final version/`: Stable, up-to-date implementation.

---
## 3. Project Structure

| Module | Description |
|--------|-------------|
| `main.cpp` | Entry point of the Qt application |
| `controlpanel.cpp/h` | GUI panel providing control buttons and data display |
| `robot.cpp/h` | Robot control logic integrating motor and sensor functions |
| `motor.cpp/h` | Motor control module with PWM speed adjustment |
| `ultrasonic.cpp/h` | Ultrasonic distance measurement module using `chrono` for precision |
| `camera.cpp/h` | Camera capture module using OpenCV |
| `CMakeLists.txt` | Build configuration for all components |

---

## 4. System Dependencies

**Hardware:**

（1）Raspberry Pi 5

（2）4 DC motors + 2 L298N motor drivers


<img width="389" height="392" alt="截屏2025-07-14 11 05 58" src="https://github.com/user-attachments/assets/b3eb1580-111b-444c-98dc-e887f6472a42" />



（3）3 HC-SR04 ultrasonic sensors


<img width="389" height="392" alt="截屏2025-07-14 11 04 13" src="https://github.com/user-attachments/assets/21546774-a35f-4c3c-a9a2-7834d84aa232" />


（4）Kamera V2 (8MP, 1080p) camera module


<img width="389" height="392" alt="截屏2025-07-14 11 03 15" src="https://github.com/user-attachments/assets/250092e1-948c-4080-8cf8-a821446ae4cf" />


Mechanical Structure of the Car

<img width="389" height="392" alt="截屏2025-07-14 11 19 20" src="https://github.com/user-attachments/assets/4d7e9d7f-c4ef-4ef5-b786-c3cdcc69fd56" />


**Software:**

- Qt 5 (qtbase5-dev, qt5-default): for GUI development and event loop integration
- OpenCV 4.x (libopencv-dev): for camera access and image processing (basic modules only)
- libgpiod (libgpiod-dev): for GPIO-based interrupt-driven ultrasonic sensing
- C++17 (recommended): ensures compatibility with modern threading and standard library features

---

## 5. System architecture
This section provides a detailed description of the hardware wiring configuration between the Raspberry Pi 5, the motor driver modules (L298N), and the ultrasonic sensors (HC-SR04). The system utilizes two L298N motor drivers to independently control the four-wheel drive motors, while three ultrasonic sensors are used for real-time obstacle detection. Each module is connected to the Raspberry Pi's GPIO pins through designated physical pin mappings to ensure stable control and data acquisition. The wiring tables below outline the specific GPIO assignments for all modules.
### L298N A Module Wiring
| Function                             | L298N Pin | GPIO (BCM) | Physical Pin |
|--------------------------------------|-----------|------------|---------------|
| Left Front Wheel Direction Control 1 | IN1       | GPIO17     | Pin 11        |
| Left Front Wheel Direction Control 2 | IN2       | GPIO18     | Pin 12        |
| Right Front Wheel Direction Control 1| IN3       | GPIO27     | Pin 13        |
| Right Front Wheel Direction Control 2| IN4       | GPIO22     | Pin 15        |
| Left Front Wheel PWM (Speed Control) | ENA       | GPIO12 (PWM)| Pin 32       |
| Right Front Wheel PWM (Speed Control)| ENB       | GPIO13 (PWM)| Pin 33       |
| Motor Power Input                    | VCC       | External Battery + | —     |
| Ground (Common)                      | GND       | GND         | Pin 6 / 14 etc|

### L298N B Module Wiring
| Function                              | L298N Pin | GPIO (BCM) | Physical Pin |
|---------------------------------------|-----------|------------|---------------|
| Left Rear Wheel Direction Control 1   | IN1       | GPIO5      | Pin 29        |
| Left Rear Wheel Direction Control 2   | IN2       | GPIO6      | Pin 31        |
| Right Rear Wheel Direction Control 1  | IN3       | GPIO19 (PWM)| Pin 35       |
| Right Rear Wheel Direction Control 2  | IN4       | GPIO26     | Pin 37        |
| Left Rear Wheel PWM (Speed Control)   | ENA       | GPIO20 (PWM)| Pin 38       |
| Right Rear Wheel PWM (Speed Control)  | ENB       | GPIO21 (PWM)| Pin 40       |
| Motor Power Input                     | VCC       | External Battery + | —     |
| Ground (Common)                       | GND       | GND         | Pin 6 / 14 etc|

### Front Ultrasonic Sensor Wiring
| Function | HC-SR04 Pin | GPIO (BCM) | Physical Pin     |
|----------|-------------|------------|------------------|
| TRIG     | TRIG        | GPIO20     | Pin 8            |
| ECHO     | ECHO        | GPIO21     | Pin 10 *(Voltage Divider)* |
| Power    | VCC         | 5V         | Pin 2 / 4        |
| Ground   | GND         | GND        | Pin 6 / 14 / 39  |

### Left Ultrasonic Sensor Wiring
| Function | HC-SR04 Pin | GPIO (BCM) | Physical Pin     |
|----------|-------------|------------|------------------|
| TRIG     | TRIG        | GPIO23     | Pin 16           |
| ECHO     | ECHO        | GPIO24     | Pin 18 *(Voltage Divider)* |
| Power    | VCC         | 5V         | Pin 2 / 4        |
| Ground   | GND         | GND        | Pin 6 / 14 / 39  |

### Right Ultrasonic Sensor Wiring
| Function | HC-SR04 Pin | GPIO (BCM) | Physical Pin     |
|----------|-------------|------------|------------------|
| TRIG     | TRIG        | GPIO25     | Pin 22           |
| ECHO     | ECHO        | GPIO26     | Pin 36 *(Voltage Divider)* |
| Power    | VCC         | 5V         | Pin 2 / 4        |
| Ground   | GND         | GND        | Pin 6 / 14 / 39  |

### Raspberry Pi 5 GPIO Pinout Diagram
The following diagram illustrates the Raspberry Pi 5 GPIO header layout, showing both the physical pin numbers (1–40) and the corresponding BCM GPIO numbers. This diagram serves as a reference for understanding the pin assignments used in the wiring tables below.
<img width="864" height="459" alt="image" src="https://github.com/user-attachments/assets/ac9bf033-7651-45ca-bcf2-648aa8b58441" />


## 6. Build & Run

###  Dependency Installation (Run Once)

```bash
sudo apt update
sudo apt install -y qtbase5-dev qttools5-dev-tools \
libopencv-dev libgpiod-dev build-essential cmake pkg-config
```


---

###  Build the Project

```bash
mkdir -p build
cd build
cmake ..
make        # Build the Qt GUI application (smartcar)
```
###  Run the Application

####  If using SSH (no desktop):

```bash
export QT_QPA_PLATFORM=offscreen
./smartcar
```

---

## 7. Real-Time Execution Model

All time-sensitive modules (such as camera capture and ultrasonic sensing) are executed on independent threads using an event-driven structure.

- **Multithreaded Design:** Each module (e.g., camera, ultrasonic) runs in its own dedicated thread to ensure responsiveness and concurrency.
- **Event-Driven Architecture:** No polling or fixed delays are used. Instead, each component operates continuously and triggers callbacks when new data is available.
- **Callback Interfaces:** 
  - Ultrasonic modules notify the main controller via `onTooClose(distance, sensorId)`.
  - The camera module sends live image frames using `onFrameCaptured(const cv::Mat& frame)`.
- **Finite State Machines (FSM):** Robot behavior is governed using FSMs to manage transitions such as READY → RUNNING → STOPPED based on input conditions.
- **Performance Monitoring:** Key task durations can be optionally measured using `std::chrono` to monitor and debug real-time responsiveness.

This architecture replaces the previously used `TaskScheduler` and ensures modular, maintainable, and soft real-time control without relying on artificial timing delays.

---

## 8. GUI Features

- **Directional control buttons**: Forward, Backward, Left, Right
- **Speed control slider**: Adjusts motor PWM output (0–1023)
- **Live camera view**: Real-time preview from the front camera
- **Ultrasonic distance display**: Shows distances from front, left, and right sensors
- **Status label**: Displays the robot’s current action (e.g., Forward, Stopped)
- **Start/Stop buttons**: Initializes or shuts down the control system

---

## 9. Project Highlights

- **Fully Modular and Object-Oriented Design:** Each component (motor, sensor, GUI, logic) is implemented as an independent class.
- **Non-Blocking, Event-Driven Real-Time Control:** Real-time responsiveness is achieved using multithreading and state machines without delay-based polling.
- **High-Precision Ultrasonic Measurement:** Timing is handled via `std::chrono` for accurate edge-based distance sensing.
- **Decoupled GUI and Control Logic:** The GUI (Qt) and backend logic (sensor and motion control) communicate cleanly through callback interfaces and signals.
- **Entirely Written in C++:** All components are implemented in modern C++ (C++17), ensuring performance and portability.
---

## 10. Screenshots / Demo

- [Bilibili demo](https://t.bilibili.com/1058982800262168584?share_source=pc_native)
- [YouTube demo](https://youtu.be/4GdFb-aglIA)
- Preview of the Qt control panel
- Hardware wiring diagram (motors, sensors, camera)

---

## 11. Team Members

- Ye He (2966900H)
- Yuan Zhao (3026815Z)
- Hao Wu (2982755W)
- Xiaoyun Ma (3010324M)
- Zhaojie Zhu (2982461Z)
