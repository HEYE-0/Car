🚗 Real-Time Disaster-Response Smart Car

1. Project Overview

This project is a practical assignment for the ENG5220: Real Time Embedded Programming course. It aims to develop a small mobile robot equipped with intelligent obstacle avoidance and target-following capabilities.

The smart car is designed to navigate complex environments such as earthquake zones and other natural disaster sites. With ultrasonic sensors and a camera, it can detect and avoid obstacles in real time. Furthermore, it attempts to recognize and follow individuals, making it useful for emergency tasks like material transport.

In addition to its autonomous functions, the car supports manual control through keyboard or web interface, providing flexibility for high-risk or precision-required environments.

The project is developed on Ubuntu, with core logic in C++ and web interface in Python, balancing real-time performance and scalability.

2. Project Structure

```
Car/
├── Arduino/                 # Arduino control scripts
├── Camera/                  # Camera image processing module
├── Obstacle_avoidance_code/ # Ultrasonic obstacle avoidance logic
├── shoe_follow_project/     # Target following functionality
├── follow_change/           # Update history of the following module
├── follow_test/             # Testing scripts for target following
├── web/                     # Web-based control interface (Flask)
├── Tutorial/                # Tutorials and documentation
├── .vscode/                 # VS Code configuration
├── venv/                    # Python virtual environment
├── .gitignore               # Git ignore configuration
└── README.md                # Project documentation

```
3. Installation & Usage

This project runs on a Raspberry Pi, supports three modes: automatic, follow (attempted), and manual control. The Arduino executes the movement control.

3.1 Startup

Power on the Raspberry Pi. It will auto-launch the main control program. You can switch modes via keyboard input.

3.2 Manual Mode (default)

Use the keyboard to send commands via serial:
	•	w: Move forward
	•	s: Move backward
	•	a: Turn left
	•	d: Turn right
	•	q: Stop

3.3 Automatic Mode
	•	p: Enter automatic obstacle avoidance
	•	m: Exit automatic mode and return to manual

3.4 Web Control

Run the Flask server in the /web folder and visit:http://172.20.10.12:8888
for a graphical interface.

3.5 Follow Mode (Not Fully Functional)

We attempted to implement target following using ArUco marker detection (ID = 5). However, due to limited camera resolution (Raspberry Pi V2 camera) and motor steering accuracy, we couldn’t achieve stable following. The system logic and test scripts remain in /follow_test/ for reference and future improvement.

⚠️ Make sure the ultrasonic modules and camera are connected and functional before using auto modes.
The Arduino must be programmed in advance over USB.
Default baud rate is 9600.
Ideal conditions: flat surface and good lighting for reliable vision.


4. Core Features
	•	Real-time ultrasonic obstacle avoidance
	•	Camera-based ArUco marker detection
	•	Seamless manual/auto mode switching
	•	Web-based control via Flask
	•	Modular code structure for easy extension


5. Tech Stack
	•	OS: Ubuntu on Raspberry Pi
	•	Languages: C++ (core), Python (web and utilities)
	•	Framework: Flask for web control
	•	Vision: OpenCV & ArUco markers
	•	Hardware: Raspberry Pi, Arduino UNO, L298N motor driver, ultrasonic sensors, Pi Camera V2
	•	Communication: Serial via /dev/ttyACM0


6. License

This project is licensed under the MIT License.
Feel free to contribute or fork for your own enhancements.


7. Author

Author: HEYE-0
GitHub: https://github.com/HEYE-0/Car
