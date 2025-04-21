Real-Time Disaster-Response Smart Car
1. Project Overview
This project is a practical assignment for the ENG5220: Real Time Embedded Programming course. It aims to develop a small mobile robot equipped with intelligent obstacle avoidance and target-following capabilities.

The smart car is designed to navigate complex environments in scenarios such as earthquakes and other natural disasters. Utilizing ultrasonic sensors and a camera, it can detect and avoid obstacles in real time. Moreover, the system can automatically recognize and follow individuals, making it suitable for emergency tasks like material transportation.

Additionally, the car supports manual control mode, allowing human intervention in high-risk or precision-required situations, enhancing the system’s overall flexibility and reliability.

The project is developed on an Ubuntu system, with core control logic written in C++, and the web control interface developed using Python, balancing real-time performance and scalability.
2. Project Structure
Car/
├── Camera/                   # Camera image processing module  
├── Obstacle avoidance_code/ # Ultrasonic obstacle avoidance control code  
├── shoe_follow_project/     # Target-following implementation  
├── follow_change/           # Follow module update logs  
├── follow_test/             # Follow feature testing scripts  
├── web/                     # Web-based remote control interface (Flask)  
├── Tutorial/                # Tutorials and user documentation  
├── .vscode/                 # VS Code configuration  
├── venv/                    # Python virtual environment  
├── .gitignore               # Git ignore rules  
└── README.md                # Project description file
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
