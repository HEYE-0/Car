
// --- Ultrasonic pin ---
#define TRIG_FRONT A1
#define ECHO_FRONT A0
#define TRIG_LEFT  A3
#define ECHO_LEFT  A2
#define TRIG_RIGHT A5
#define ECHO_RIGHT A4

// --- motor pins ---
#define M1A_IN1 2// --- Ultrasonic Pins --- 
#define TRIG_FRONT A1  // GPIO pin connected to the Trig of the front ultrasonic sensor
#define ECHO_FRONT A0  // GPIO pin connected to the Echo of the front ultrasonic sensor
#define TRIG_LEFT  A3  // GPIO pin connected to the Trig of the left ultrasonic sensor
#define ECHO_LEFT  A2  // GPIO pin connected to the Echo of the left ultrasonic sensor
#define TRIG_RIGHT A5  // GPIO pin connected to the Trig of the right ultrasonic sensor
#define ECHO_RIGHT A4  // GPIO pin connected to the Echo of the right ultrasonic sensor

// --- Motor Pins ---
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

int speedMotor = 120;
char mode = 'p';       // Current mode: p=automatic, m=manual, f=follow
char follow_cmd = 'q'; // Command in follow mode (from Raspberry Pi)

// Control function encapsulation
void motor(int in1, int in2, int en, int s1, int s2, int pwm = speedMotor) {
  digitalWrite(in1, s1);
  digitalWrite(in2, s2);
  analogWrite(en, pwm);
}

void setup() {
  Serial.begin(9600);
  pinMode(M1A_IN1, OUTPUT); pinMode(M1A_IN2, OUTPUT); pinMode(M1A_EN, OUTPUT);
  pinMode(M1B_IN1, OUTPUT); pinMode(M1B_IN2, OUTPUT); pinMode(M1B_EN, OUTPUT);
  pinMode(M2A_IN1, OUTPUT); pinMode(M2A_IN2, OUTPUT); pinMode(M2A_EN, OUTPUT);
  pinMode(M2B_IN1, OUTPUT); pinMode(M2B_IN2, OUTPUT); pinMode(M2B_EN, OUTPUT);

  pinMode(TRIG_FRONT, OUTPUT); pinMode(ECHO_FRONT, INPUT);
  pinMode(TRIG_LEFT, OUTPUT);  pinMode(ECHO_LEFT, INPUT);
  pinMode(TRIG_RIGHT, OUTPUT); pinMode(ECHO_RIGHT, INPUT);

  stopMotors();
  Serial.println("✅ Arduino Ready. Default: Auto mode");
}

void loop() {
  // Serial port mode switching or control commands
  if (Serial.available()) {
    char cmd = Serial.read();

    if (cmd == 'p' || cmd == 'm' || cmd == 'f') {
      mode = cmd;
      Serial.print("🔁 Mode switched to: "); Serial.println(mode);
    } else if (mode == 'm') {
      handleManual(cmd);
    } else if (mode == 'f') {
      follow_cmd = cmd;
    }
  }

  if (mode == 'p') handleAuto();
  else if (mode == 'f') handleFollow();
}

// ========== Control Functions =========

void handleManual(char cmd) {
  switch (cmd) {
    case 'w': moveForward(); break;
    case 's': moveBackward(); break;
    case 'a': turnLeft(); break;
    case 'd': turnRight(); break;
    case 'q': stopMotors(); break;
  }
}

void handleFollow() {
  switch (follow_cmd) {
    case 'w': moveForward(); delay(50); stopMotors(); break;
    case 'a': turnLeft(); delay(50); stopMotors(); break;
    case 'd': turnRight(); delay(50); stopMotors(); break;
    case 'q': stopMotors(); break;
  }
}

void handleAuto() {
  float front = getDistance(TRIG_FRONT, ECHO_FRONT);
  float left  = getDistance(TRIG_LEFT,  ECHO_LEFT);
  float right = getDistance(TRIG_RIGHT, ECHO_RIGHT);

  if (front > 0 && front < 20) {
    stopMotors(); delay(200);
    moveBackward(); delay(400);
    turnRight(); delay(400);
  } else if (left > 0 && left < 15) {
    turnRight(); delay(300);
  } else if (right > 0 && right < 15) {
    turnLeft(); delay(300);
  } else {
    moveForward();
  }

  delay(100);
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

void stopMotors() {
  motor(M1A_IN1, M1A_IN2, M1A_EN, LOW, LOW, 0);
  motor(M1B_IN1, M1B_IN2, M1B_EN, LOW, LOW, 0);
  motor(M2A_IN1, M2A_IN2, M2A_EN, LOW, LOW, 0);
  motor(M2B_IN1, M2B_IN2, M2B_EN, LOW, LOW, 0);
}

// ========== Distance Measurement Function =========
float getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW); delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH, 25000);
  if (duration == 0) return -1;
  return duration * 0.034 / 2.0;
}

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

int speedMotor = 120;
char mode = 'p';       // Current mode: p = automatic, m = manual, f = follow
char follow_cmd = 'q'; // The instructions in follow - mode (from the Raspberry Pi)

// Encapsulation of control functions
void motor(int in1, int in2, int en, int s1, int s2, int pwm = speedMotor) {
  digitalWrite(in1, s1);
  digitalWrite(in2, s2);
  analogWrite(en, pwm);
}

void setup() {
  Serial.begin(9600);
  pinMode(M1A_IN1, OUTPUT); pinMode(M1A_IN2, OUTPUT); pinMode(M1A_EN, OUTPUT);
  pinMode(M1B_IN1, OUTPUT); pinMode(M1B_IN2, OUTPUT); pinMode(M1B_EN, OUTPUT);
  pinMode(M2A_IN1, OUTPUT); pinMode(M2A_IN2, OUTPUT); pinMode(M2A_EN, OUTPUT);
  pinMode(M2B_IN1, OUTPUT); pinMode(M2B_IN2, OUTPUT); pinMode(M2B_EN, OUTPUT);

  pinMode(TRIG_FRONT, OUTPUT); pinMode(ECHO_FRONT, INPUT);
  pinMode(TRIG_LEFT, OUTPUT);  pinMode(ECHO_LEFT, INPUT);
  pinMode(TRIG_RIGHT, OUTPUT); pinMode(ECHO_RIGHT, INPUT);

  stopMotors();
  Serial.println("✅ Arduino Ready. Default: Auto mode");
}

void loop() {
  // 串口接收Switching of serial port receiving mode or control command模式切换或控制命令
  if (Serial.available()) {
    char cmd = Serial.read();

    if (cmd == 'p' || cmd == 'm' || cmd == 'f') {
      mode = cmd;
      Serial.print("🔁 Mode switched to: "); Serial.println(mode);
    } else if (mode == 'm') {
      handleManual(cmd);
    } else if (mode == 'f') {
      follow_cmd = cmd;
    }
  }

  if (mode == 'p') handleAuto();
  else if (mode == 'f') handleFollow();
}

// ========== Control function ==========

void handleManual(char cmd) {
  switch (cmd) {
    case 'w': moveForward(); break;
    case 's': moveBackward(); break;
    case 'a': turnLeft(); break;
    case 'd': turnRight(); break;
    case 'q': stopMotors(); break;
  }
}

void handleFollow() {
  switch (follow_cmd) {
    case 'w': moveForward(); delay(50); stopMotors(); break;
    case 'a': turnLeft(); delay(50); stopMotors(); break;
    case 'd': turnRight(); delay(50); stopMotors(); break;
    case 'q': stopMotors(); break;
  }
}

void handleAuto() {
  float front = getDistance(TRIG_FRONT, ECHO_FRONT);
  float left  = getDistance(TRIG_LEFT,  ECHO_LEFT);
  float right = getDistance(TRIG_RIGHT, ECHO_RIGHT);

  if (front > 0 && front < 20) {
    stopMotors(); delay(200);
    moveBackward(); delay(400);
    turnRight(); delay(400);
  } else if (left > 0 && left < 15) {
    turnRight(); delay(300);
  } else if (right > 0 && right < 15) {
    turnLeft(); delay(300);
  } else {
    moveForward();
  }

  delay(100);
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

void stopMotors() {
  motor(M1A_IN1, M1A_IN2, M1A_EN, LOW, LOW, 0);
  motor(M1B_IN1, M1B_IN2, M1B_EN, LOW, LOW, 0);
  motor(M2A_IN1, M2A_IN2, M2A_EN, LOW, LOW, 0);
  motor(M2B_IN1, M2B_IN2, M2B_EN, LOW, LOW, 0);
}

// ========== Distance - measuring function ==========
float getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW); delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH, 25000);
  if (duration == 0) return -1;
  return duration * 0.034 / 2.0;
}
