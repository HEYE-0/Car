# 🚗 Bluetooth-Based Obstacle Avoidance Car - Experiment Log (Version 1)

> 📅 Date: June 20, 2025  
> 🧪 Code Version: Version 1  
> 🧠 Project Title: Bluetooth-Based Obstacle Avoidance Car  
> 🎯 Purpose: To validate the basic functionality of obstacle avoidance logic and Bluetooth communication

---

## 🔧 Experimental Setup

| Component | Description |
|-----------|-------------|
| Controller | Raspberry Pi |
| Sensors | Ultrasonic sensor (HC-SR04) |
| Communication | Bluetooth Serial Module HC-05 |
| Motor Driver | L298N Dual H-Bridge Motor Driver |
| Car Structure | 3-wheel platform (2 drive wheels + 1 universal wheel) |
| Power Supply | 7.4V Li-ion Battery |
| Control Interface | Android Bluetooth Terminal App |

---

## 🧪 Test Procedure

1. **Bluetooth Connectivity Test**
   - Open Bluetooth terminal app on phone and connect to HC-05
   - Message `"Connected"` received successfully

2. **Obstacle Avoidance Test**
   - Upload Version 1 code to the project
   - Power on the car, it starts moving
   - Ultrasonic sensor measures distance in front:
     - If distance > 25cm: move forward
     - If distance ≤ 25cm: stop and turn (random left/right)

3. **Manual Control Test**
   - Send commands via Bluetooth (e.g., `F` for forward, `B` for backward)
   - Commands are received and executed, but conflict with auto mode

---

## ✅ Summary of Results

| Item | Outcome |
|------|---------|
| Bluetooth connection | Successful |
| Manual command response | Successful |
| Obstacle avoidance logic | Partially effective |
| Car behavior | Can detect and avoid obstacles to some extent |

---

## ⚠️ Issues Encountered

### 1. 🚧 Inaccurate Obstacle Detection

- **Problem**: Car sometimes failed to detect obstacles and crashed
- **Possible Causes**:
  - Ultrasonic sensor mounted too high, missing low objects
  - No filtering applied to distance measurements

### 2. ↪️ Unstable Turning Behavior

- **Problem**: Turns were either too sharp or too small
- **Possible Causes**:
  - Uneven PWM signal to motors
  - Fixed turning duration not adaptive to speed

### 3. 🔁 Conflict Between Manual and Auto Modes

- **Problem**: When `F` is sent manually, car does not auto-avoid obstacles
- **Possible Causes**:
  - Manual control lacks mode switching logic
  - Auto and manual code not modularized

---

## 🛠️ Suggestions for Improvement (for Version 2)

1. **Improved Detection**
   - Adjust sensor angle to cover low-range
   - Apply filters like median filter to ultrasonic readings

2. **Refined Turning Logic**
   - Use dynamic adjustment for turning time
   - Optionally integrate gyroscope (e.g., MPU6050)

3. **Mode Management**
   - Add mode toggle commands (`AUTO`, `MANUAL`)
   - Run obstacle logic only in AUTO mode

4. **Code Refactoring**
   - Separate logic into sensing layer and control layer
   - Modularize Bluetooth handler and obstacle handler

---

## 📸 Additional Notes

> 💡 Consider adding a camera or ArUco marker recognition in future versions to enhance obstacle avoidance

---

## 🧾 Commit Record

- Code version: `v1.0_initial_release`
- Uploaded by: zygzzy
- Timestamp: 2025-06-20 17:42
- Commit Message: Add files via upload

---

## 🏁 Conclusion

Version 1 of the Bluetooth-based obstacle avoidance car demonstrates basic functionality but still requires improvements in detection accuracy and turning control. The next version will focus on refining logic and hardware integration for more stable behavior.
