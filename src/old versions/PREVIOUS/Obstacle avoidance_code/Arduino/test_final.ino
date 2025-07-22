// === Pin Definitions ===
// Ultrasonic sensors
#define TRIG_FRONT A1  // Pin connected to the Trig of the front ultrasonic sensor
#define ECHO_FRONT A0  // Pin connected to the Echo of the front ultrasonic sensor
#define TRIG_LEFT  A3  // Pin connected to the Trig of the left ultrasonic sensor
#define ECHO_LEFT  A2  // Pin connected to the Echo of the left ultrasonic sensor
#define TRIG_RIGHT A5  // Pin connected to the Trig of the right ultrasonic sensor
#define ECHO_RIGHT A4  // Pin connected to the Echo of the right ultrasonic sensor

// Motor pins
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

// === Control Variables ===
int speedMotor = 80;
char mode = 'm';  // Default mode is manual

// Obstacle state machine
enum ObstacleState { NORMAL, BACKING, TURNING };
ObstacleState obsState = NORMAL;
unsigned long stateStartTime = 0;
unsigned long lastCheckTime = 0;
const unsigned long checkInterval = 100;

// === Motor Control Functions ===
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

// === Ultrasonic Distance Measurement Function (in cm) ===
float getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  digitalWrite(trigPin, HIGH);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 25000); // Timeout at 25ms
  if (duration == 0) return -1;
  return duration * 0.034 / 2.0;
}

// === Obstacle Avoidance State Control Logic ===
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
        turnRight();  // Or you can decide the direction based on left/right distances
      } else {
        obsState = NORMAL;
      }
      break;
  }
}

// === Initialization Setup ===
void setup() {
  Serial.begin(9600);

  // Initialize motors
  pinMode(M1A_IN1, OUTPUT); pinMode(M1A_IN2, OUTPUT); pinMode(M1A_EN, OUTPUT);
  pinMode(M1B_IN1, OUTPUT); pinMode(M1B_IN2, OUTPUT); pinMode(M1B_EN, OUTPUT);
  pinMode(M2A_IN1, OUTPUT); pinMode(M2A_IN2, OUTPUT); pinMode(M2A_EN, OUTPUT);
  pinMode(M2B_IN1, OUTPUT); pinMode(M2B_IN2, OUTPUT); pinMode(M2B_EN, OUTPUT);

  // Initialize sensors
  pinMode(TRIG_FRONT, OUTPUT); pinMode(ECHO_FRONT, INPUT);
  pinMode(TRIG_LEFT, OUTPUT);  pinMode(ECHO_LEFT, INPUT);
  pinMode(TRIG_RIGHT, OUTPUT); pinMode(ECHO_RIGHT, INPUT);

  stopMotors();
  Serial.println("✅ Arduino Ready (Auto mode default)");
}

// === Main Loop Logic ===
void loop() {
  // Switch between serial modes and manual control
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

  // Auto obstacle avoidance mode logic
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
        avoidObstacle();  // State machine in progress
      } else {
        if (front > 0 && front < 20) {
          obsState = BACKING;  // Start state machine
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
