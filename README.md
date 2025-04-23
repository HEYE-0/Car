Real-Time Disaster-Response Smart Car
1. Project Overview
This project is a hands-on assignment for the ENG5220: Real Time Embedded Programming course, aimed at developing a small mobile robot with intelligent obstacle avoidance and target-following capabilities. The primary goal is to create a robot that can operate autonomously in complex environments, such as those encountered during earthquakes or other natural disasters, where navigating unpredictable and hazardous terrain is crucial.

The smart car is designed to be versatile, equipped with ultrasonic sensors and a camera to detect and avoid obstacles in real time, ensuring it can move smoothly through challenging spaces. In addition, the robot has the ability to automatically identify and follow individuals, making it particularly useful for emergency situations, like transporting materials in disaster zones or providing assistance in search-and-rescue operations.

To further enhance its functionality, the car also supports manual control mode. This feature allows a human operator to intervene in high-risk situations or when precise movements are required, providing additional flexibility and reliability to the system. 

The entire project is developed on an Ubuntu system, with the core control logic written in C++ to ensure efficient, real-time performance. The web control interface is built using Python, striking a balance between scalability and the ability to manage the system remotely, providing a robust solution for real-world applications.
2. Project Structure
Car/
├── Camera/                       # Camera image processing module  
├── Obstacle_Avoidance_Code/       # Ultrasonic obstacle avoidance control code  
├── Target_Following_Project/      # Implementation of target-following functionality  
├── Follow_Module_Updates/         # Logs of updates made to the follow module  
├── Follow_Testing/                # Scripts for testing the follow feature  
├── Web_Interface/                 # Web-based remote control interface (Flask)  
├── Documentation/                 # Tutorials and user documentation  
├── .vscode/                       # VS Code configuration files  
├── venv/                          # Python virtual environment  
├── .gitignore                     # Git ignore rules  
└── README.md                      # Project description and overview file  
3. Installation & Usage
The project is deployed on a Raspberry Pi system and supports three control modes: automatic obstacle avoidance, target following, and manual remote control. The core motion control is executed via an Arduino.

Basic usage steps:

3.1 Power on and start the Raspberry Pi system  
Upon startup, the Raspberry Pi automatically enters the main control program (you can manually switch modes via command line).

3.2 Default to manual control mode  
Use serial commands to control the car's movements:
- 'w': move forward
- 's': move backward
- 'a': turn left
- 'd': turn right
- 'q': stop

3.3 Switch to automatic mode
- 'p': enter automatic obstacle avoidance/following mode
- 'm': exit automatic mode and return to manual control

3.4 Web control
After launching the web service on the Raspberry Pi, you can visit:
http://<Raspberry_Pi_IP>:5000
to access the graphical control interface (startup script is located in the /web directory).

3.5 Follow mode  
Run follow_test/main, and the car will detect and attempt to follow an object in front, suitable for tracking a person's feet or other designated targets.

Notes:
- Ensure the ultrasonic sensors and camera are functioning properly before using automatic mode.
- Arduino control logic must be uploaded via USB in advance.
- Default serial baud rate is 9600.
- Recommended usage environment: flat ground with good lighting to ensure accurate visual recognition.
4. Core Features
- Real-time ultrasonic obstacle avoidance  
- Visual recognition and target following via camera  
- Seamless switching between manual and automatic modes  
- Flask-based web control interface  
- Modular design for easy expansion and integration
5. Project Demonstration
<!-- Replace the link below with demo video/image if available -->
6. Tech Stack
- Development Platform: Ubuntu  
- Programming Languages: C++ (core control), Python (web & script logic)  
- Hardware Platform: Raspberry Pi, Arduino UNO, L298N, ultrasonic modules, camera  
- Vision Processing: OpenCV  
- Control System: Flask + serial communication
7. Open Source License
This project is open-sourced under the MIT License. Contributions and further development are welcome.
8. Author Information
Author: HEYE-0  
GitHub: https://github.com/HEYE-0/Car
