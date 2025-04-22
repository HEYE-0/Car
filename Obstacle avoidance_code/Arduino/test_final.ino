// === 引脚定义 ===
// 超声波传感器
#define TRIG_FRONT A1
#define ECHO_FRONT A0
#define TRIG_LEFT  A3
#define ECHO_LEFT  A2
#define TRIG_RIGHT A5
#define ECHO_RIGHT A4

// 电机引脚
#define M1A_IN1 2
#define M1A_IN2 4
#define M1A_EN 3

#define M1B_IN1 7
#define M1B_IN2 6
#define M1B_EN 5

#define M2A_IN1 9
#define M2A_IN2 8
#define M2A_EN 10

#define M2B_IN1 13
#define M2B_IN2 12
#define M2B_EN 11

// === 控制变量 ===
int speedMotor = 80;
char mode = 'm';  // 默认手动模式

// 避障状态机
enum ObstacleState { NORMAL, BACKING, TURNING };
ObstacleState obsState = NORMAL;
unsigned long stateStartTime = 0;
unsigned long lastCheckTime = 0;
const unsigned long checkInterval = 100;

// === 电机控制函数 ===
void motor(int in1, int in2, int en, int s1, int s2, int pwm = speedMotor) {
  digitalWrite(in1, s1);
  digitalWrite(in2, s2);
  analogWrite(en, pwm);
}

void stopMotors() {
  motor(M1A_IN1, M1A_IN2, M1A_EN, LOW, LOW, 0);
  motor(M1B_IN1, M1B_IN2, M1B_EN, LOW, LOW, 0);
  motor(M2A_IN1, M2A_IN2, M2A_EN, LOW, LOW, 0);
  motor(M2B_IN1, M2B_IN2, M2B_EN, LOW, LOW, 0);
}

void moveForward() {
  motor(M1A_IN1, M1A_IN2, M1A_EN, LOW, HIGH);
  motor(M1B_IN1, M1B_IN2, M1B_EN, HIGH, LOW);
  motor(M2A_IN1, M2A_IN2, M2A_EN, LOW, HIGH);
  motor(M2B_IN1, M2B_IN2, M2B_EN, LOW, HIGH);
}

void moveBackward() {
  motor(M1A_IN1, M1A_IN2, M1A_EN, HIGH, LOW);
  motor(M1B_IN1, M1B_IN2, M1B_EN, LOW, HIGH);
  motor(M2A_IN1, M2A_IN2, M2A_EN, HIGH, LOW);
  motor(M2B_IN1, M2B_IN2, M2B_EN, HIGH, LOW);
}

void turnLeft() {
  motor(M1A_IN1, M1A_IN2, M1A_EN, HIGH, LOW);
  motor(M1B_IN1, M1B_IN2, M1B_EN, HIGH, LOW);
  motor(M2A_IN1, M2A_IN2, M2A_EN, HIGH, LOW);
  motor(M2B_IN1, M2B_IN2, M2B_EN, LOW, HIGH);
}

void turnRight() {
  motor(M1A_IN1, M1A_IN2, M1A_EN, LOW, HIGH);
  motor(M1B_IN1, M1B_IN2, M1B_EN, LOW, HIGH);
  motor(M2A_IN1, M2A_IN2, M2A_EN, LOW, HIGH);
  motor(M2B_IN1, M2B_IN2, M2B_EN, HIGH, LOW);
}

// === 超声波测距函数（单位 cm）===
float getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 25000); // 超时 25ms
  if (duration == 0) return -1;
  return duration * 0.034 / 2.0;
}

// === 避障状态控制逻辑 ===
void avoidObstacle() {
  unsigned long now = millis();
  switch (obsState) {
    case NORMAL:
      stopMotors();
      obsState = BACKING;
      stateStartTime = now;
      break;
    case BACKING:
      if (now - stateStartTime < 400) {
        moveBackward();
      } else {
        obsState = TURNING;
        stateStartTime = now;
      }
      break;
    case TURNING:
      if (now - stateStartTime < 400) {
        turnRight();  // 或可根据左右距离决定方向
      } else {
        obsState = NORMAL;
      }
      break;
  }
}

// === 初始化设置 ===
void setup() {
  Serial.begin(9600);

  // 初始化电机
  pinMode(M1A_IN1, OUTPUT); pinMode(M1A_IN2, OUTPUT); pinMode(M1A_EN, OUTPUT);
  pinMode(M1B_IN1, OUTPUT); pinMode(M1B_IN2, OUTPUT); pinMode(M1B_EN, OUTPUT);
  pinMode(M2A_IN1, OUTPUT); pinMode(M2A_IN2, OUTPUT); pinMode(M2A_EN, OUTPUT);
  pinMode(M2B_IN1, OUTPUT); pinMode(M2B_IN2, OUTPUT); pinMode(M2B_EN, OUTPUT);

  // 初始化传感器
  pinMode(TRIG_FRONT, OUTPUT); pinMode(ECHO_FRONT, INPUT);
  pinMode(TRIG_LEFT, OUTPUT);  pinMode(ECHO_LEFT, INPUT);
  pinMode(TRIG_RIGHT, OUTPUT); pinMode(ECHO_RIGHT, INPUT);

  stopMotors();
  Serial.println("✅ Arduino Ready (Auto mode default)");
}

// === 主循环逻辑 ===
void loop() {
  // 串口模式切换与手动控制
  if (Serial.available()) {
    char cmd = Serial.read();
    switch (cmd) {
      case 'p': mode = 'a'; Serial.println("🚗 Auto Mode Enabled"); break;
      case 'o': mode = 'm'; Serial.println("🎮 Manual Mode Enabled"); break;
      case 'w': if (mode == 'm') moveForward(); break;
      case 's': if (mode == 'm') moveBackward(); break;
      case 'a': if (mode == 'm') turnLeft(); break;
      case 'd': if (mode == 'm') turnRight(); break;
      case 'q': stopMotors(); break;
    }
  }

  // 自动避障模式逻辑
  if (mode == 'a') {
    unsigned long now = millis();
    if (now - lastCheckTime >= checkInterval) {
      lastCheckTime = now;

      float front = getDistance(TRIG_FRONT, ECHO_FRONT);
      float left  = getDistance(TRIG_LEFT, ECHO_LEFT);
      float right = getDistance(TRIG_RIGHT, ECHO_RIGHT);

      Serial.print("📏 Front: "); Serial.print(front);
      Serial.print(" cm | Left: "); Serial.print(left);
      Serial.print(" cm | Right: "); Serial.println(right);

      if (obsState != NORMAL) {
        avoidObstacle();  // 状态机进行中
      } else {
        if (front > 0 && front < 20) {
          obsState = BACKING;  // 启动状态机
          stateStartTime = now;
        } else if (left > 0 && left < 15) {
          turnRight();
        } else if (right > 0 && right < 15) {
          turnLeft();
        } else {
          moveForward();
        }
      }
    }
  }
}
