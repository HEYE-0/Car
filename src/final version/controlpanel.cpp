#include "controlpanel.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>

ControlPanel::ControlPanel(QWidget *parent)
    : QWidget(parent),
      robot(new Robot()),
      currentState(RobotState::READY)
{
    setupUI();

    // 注册超声波回调
    for (int i = 0; i < 3; ++i) {
        robot->getUltrasonic(i)->registerCallback(this);
    }

    // 注册摄像头回调
    robot->getCamera()->registerCallback(this);
}

ControlPanel::~ControlPanel() {
    delete robot;
}

void ControlPanel::setupUI() {
    startButton = new QPushButton("Start", this);
    stopButton = new QPushButton("Stop", this);
    forwardButton = new QPushButton("Forward", this);
    backwardButton = new QPushButton("Backward", this);
    leftButton = new QPushButton("Left", this);
    rightButton = new QPushButton("Right", this);
    haltButton = new QPushButton("Halt", this);
    speedSlider = new QSlider(Qt::Horizontal, this);
    statusLabel = new QLabel("Ready", this);

    // 布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *controlLayout = new QHBoxLayout();

    controlLayout->addWidget(startButton);
    controlLayout->addWidget(stopButton);
    controlLayout->addWidget(forwardButton);
    controlLayout->addWidget(backwardButton);
    controlLayout->addWidget(leftButton);
    controlLayout->addWidget(rightButton);
    controlLayout->addWidget(haltButton);

    mainLayout->addLayout(controlLayout);
    mainLayout->addWidget(speedSlider);
    mainLayout->addWidget(statusLabel);

    setLayout(mainLayout);

    // 连接信号槽
    connect(startButton, &QPushButton::clicked, this, &ControlPanel::onStartClicked);
    connect(stopButton, &QPushButton::clicked, this, &ControlPanel::onStopClicked);
    connect(forwardButton, &QPushButton::clicked, this, &ControlPanel::onForwardClicked);
    connect(backwardButton, &QPushButton::clicked, this, &ControlPanel::onBackwardClicked);
    connect(leftButton, &QPushButton::clicked, this, &ControlPanel::onLeftClicked);
    connect(rightButton, &QPushButton::clicked, this, &ControlPanel::onRightClicked);
    connect(haltButton, &QPushButton::clicked, this, &ControlPanel::onHaltClicked);
}

void ControlPanel::onStartClicked() {
    statusLabel->setText("Robot Started");
    currentState = RobotState::RUNNING;
}

void ControlPanel::onStopClicked() {
    robot->stopAll();
    statusLabel->setText("Stopped");
    currentState = RobotState::STOPPED;
}

void ControlPanel::onForwardClicked() {
    robot->moveForward(speedSlider->value());
    statusLabel->setText("Moving Forward");
}

void ControlPanel::onBackwardClicked() {
    // 可以自行实现 backward 方法
    robot->stopAll();
    statusLabel->setText("Stopped for Backward Placeholder");
}

void ControlPanel::onLeftClicked() {
    robot->turnLeft(speedSlider->value());
    statusLabel->setText("Turning Left");
}

void ControlPanel::onRightClicked() {
    robot->turnRight(speedSlider->value());
    statusLabel->setText("Turning Right");
}

void ControlPanel::onHaltClicked() {
    robot->stopAll();
    statusLabel->setText("Halted");
}

void ControlPanel::onTooClose(float distance, int sensorId) {
    qDebug() << "Obstacle too close on sensor" << sensorId << "distance:" << distance;
    robot->stopAll();
    statusLabel->setText("Obstacle Detected!");
}

void ControlPanel::onMarkerDetected(int markerId, cv::Point2f pos) {
    qDebug() << "Marker detected:" << markerId << "at position:" << pos.x << "," << pos.y;
    statusLabel->setText("Marker Found!");
}
