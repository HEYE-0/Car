#include "controlpanel.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>

ControlPanel::ControlPanel(QWidget *parent) : QWidget(parent), robot(new Robot()), currentState(RobotState::READY) {
    setupUI();
    robot->getUltrasonic()->registerCallback(this);
    robot->getCamera()->registerCallback(this);
}

ControlPanel::~ControlPanel() {
    delete robot;
}

void ControlPanel::setupUI() {
    startButton = new QPushButton("Start");
    stopButton = new QPushButton("Stop");
    forwardButton = new QPushButton("Forward");
    backwardButton = new QPushButton("Backward");
    leftButton = new QPushButton("Left");
    rightButton = new QPushButton("Right");
    haltButton = new QPushButton("Halt");
    speedSlider = new QSlider(Qt::Horizontal);
    speedSlider->setRange(0, 100);
    statusLabel = new QLabel("Status: Ready");

    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *controlLayout = new QHBoxLayout;

    controlLayout->addWidget(leftButton);
    controlLayout->addWidget(forwardButton);
    controlLayout->addWidget(backwardButton);
    controlLayout->addWidget(rightButton);

    mainLayout->addLayout(controlLayout);
    mainLayout->addWidget(speedSlider);
    mainLayout->addWidget(startButton);
    mainLayout->addWidget(stopButton);
    mainLayout->addWidget(haltButton);
    mainLayout->addWidget(statusLabel);

    setLayout(mainLayout);

    connect(startButton, &QPushButton::clicked, this, &ControlPanel::onStartClicked);
    connect(stopButton, &QPushButton::clicked, this, &ControlPanel::onStopClicked);
    connect(forwardButton, &QPushButton::clicked, this, &ControlPanel::onForwardClicked);
    connect(backwardButton, &QPushButton::clicked, this, &ControlPanel::onBackwardClicked);
    connect(leftButton, &QPushButton::clicked, this, &ControlPanel::onLeftClicked);
    connect(rightButton, &QPushButton::clicked, this, &ControlPanel::onRightClicked);
    connect(haltButton, &QPushButton::clicked, this, &ControlPanel::onHaltClicked);
}

void ControlPanel::onStartClicked() {
    currentState = RobotState::RUNNING;
    statusLabel->setText("Status: Running");
}

void ControlPanel::onStopClicked() {
    currentState = RobotState::STOPPED;
    robot->stopAll();
    statusLabel->setText("Status: Stopped");
}

void ControlPanel::onForwardClicked() {
    currentState = RobotState::RUNNING;
    int speed = speedSlider->value();
    robot->moveForward(speed);
    statusLabel->setText("Status: Forward");
}

void ControlPanel::onBackwardClicked() {
    currentState = RobotState::RUNNING;
    int speed = speedSlider->value();
    robot->turnLeft(speed);
    robot->turnLeft(speed);
    statusLabel->setText("Status: Backward");
}

void ControlPanel::onLeftClicked() {
    currentState = RobotState::RUNNING;
    int speed = speedSlider->value();
    robot->turnLeft(speed);
    statusLabel->setText("Status: Left");
}

void ControlPanel::onRightClicked() {
    currentState = RobotState::RUNNING;
    int speed = speedSlider->value();
    robot->turnRight(speed);
    statusLabel->setText("Status: Right");
}

void ControlPanel::onHaltClicked() {
    currentState = RobotState::STOPPED;
    robot->stopAll();
    statusLabel->setText("Status: Halt");
}

void ControlPanel::onTooClose(float distance, int sensorId) {
    currentState = RobotState::OBSTACLE_AVOID;
    robot->stopAll();
    statusLabel->setText(QString("Obstacle %1cm on sensor %2").arg(distance).arg(sensorId));
}

void ControlPanel::onMarkerDetected(int markerId, cv::Point2f pos) {
    currentState = RobotState::FOLLOWING;
    robot->followMarker(markerId, pos);
    statusLabel->setText(QString("Following marker %1").arg(markerId));
}
