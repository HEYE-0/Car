🚗 Real-Time Disaster-Response Smart Car

1. Project Overview

This project is a hands-on assignment for the ENG5220: Real-Time Embedded Programming course, with the goal of developing a small mobile robot equipped with advanced intelligent capabilities, including obstacle avoidance and target-following. 

The smart car is specifically designed to operate in complex and dynamic environments, such as disaster zones following earthquakes or other natural disasters. Using a combination of ultrasonic sensors and a camera, it is capable of detecting and avoiding obstacles in real time, ensuring safe navigation. Additionally, the robot features target-following functionality, allowing it to recognize and track individuals. This makes it particularly useful for emergency tasks, such as transporting materials or assisting in search and rescue operations.

Beyond its autonomous features, the car also includes manual control options through either a keyboard or a web interface. This dual-control capability adds significant flexibility, enabling precise human intervention in high-risk situations or environments where careful maneuvering is required.

The project is developed on the Ubuntu operating system, with the core control logic implemented in C++ for efficiency and real-time performance, while the web interface is built using Python, providing a balance between scalability and ease of user interaction.

2. Project Structure

```
Car/
├── Arduino/                 # Scripts to control the Arduino board, handling sensor data and motor commands
├── Camera/                  # Module responsible for processing camera images, including object recognition and tracking
├── Obstacle_avoidance_code/ # Logic for ultrasonic sensor-based obstacle detection and avoidance
├── shoe_follow_project/     # Core functionality for target-following, focusing on tracking and following a moving target
├── follow_change/           # Record of updates and improvements made to the target-following module
├── follow_test/             # Scripts used to test and validate the target-following functionality in various scenarios
├── web/                     # Web-based control interface built with Flask, enabling remote monitoring and control of the car
├── Tutorial/                # Collection of tutorials and user documentation to assist with understanding and using the system
├── .vscode/                 # Configuration files for Visual Studio Code, optimized for the project’s environment
├── venv/                    # Python virtual environment containing the necessary libraries for the project’s dependencies
├── .gitignore               # Configuration file specifying which files and directories should be ignored by Git
├── Project Task Allocation.docx # Document detailing task distribution and team responsibilities
└── README.md                # Comprehensive documentation about the project, its objectives, and how to use it


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
    • Real-time ultrasonic obstacle avoidance: Enables the robot to detect and avoid obstacles in real-time using ultrasonic sensors, ensuring smooth navigation.
    • Camera-based ArUco marker detection: Uses a camera to detect ArUco markers, allowing the robot to recognize and interact with objects or targets in its environment.
    • Seamless manual/auto mode switching: Provides the flexibility to switch between manual control and autonomous operation modes, ensuring adaptability to different situations.
    • Web-based control via Flask: Offers a user-friendly web interface for remote control and monitoring, built using the Flask framework.
    • Modular code structure for easy extension: Designed with a modular architecture, allowing for easy addition of new features or adjustments to existing functionality.



5. Tech Stack
	•	OS: Ubuntu on Raspberry Pi
	•	Languages: C++ (core), Python (web and utilities)
	•	Framework: Flask for web control
	•	Vision: OpenCV & ArUco markers
	•	Hardware: Raspberry Pi, Arduino UNO, L298N motor driver, ultrasonic sensors, Pi Camera V2
	•	Communication: Serial via /dev/ttyACM0

6.Hardware Architecture Overview

This project implements an intelligent smart car with autonomous obstacle avoidance and attempted target tracking capabilities. The hardware system is designed with the following components:

	•	Drive System:
The car is powered by four DC motors, each connected to a wheel to provide independent movement. This setup enables precise control for directional movement such as forward, backward, turning, and stopping.

	•	Motor Control:
Two L298N motor driver modules are used to control the four motors. Each L298N controls two motors, and together they provide full bidirectional control for the car’s movement.

	•	Microcontroller (Arduino):
An Arduino UNO serves as the intermediary controller. It receives high-level commands from the Raspberry Pi via serial communication and translates them into low-level motor control signals for the L298N modules.

	•	Main Controller (Raspberry Pi):
The Raspberry Pi acts as the central processing unit of the system. It is responsible for:

	•	Sending movement commands to the Arduino
	•	Processing visual data from the camera (ArUco marker recognition)
	•	Hosting a remote web control interface
	•	Managing control mode switching (manual, automatic, follow)

This layered hardware design ensures a clear separation of responsibilities, where the Arduino handles low-level motor control, and the Raspberry Pi focuses on high-level decision-making and remote connectivity.

7. License

This project is licensed under the MIT License.
Feel free to contribute or fork for your own enhancements.


8. Author

Author: HEYE-0
GitHub: https://github.com/HEYE-0/Car
