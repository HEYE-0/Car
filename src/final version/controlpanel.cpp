#include "controlpanel.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>
#include <QImage>
#include <QPixmap>
#include <opencv2/imgproc.hpp>

ControlPanel::ControlPanel(QWidget *parent)
    : QWidget(parent),
      robot(new Robot()),
      currentState(RobotState::READY)
{
    setupUI();

    for (int i = 0; i < 3; ++i) {
        robot->getUltrasonic(i)->registerCallback(this);
    }

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
    cameraViewLabel = new QLabel(this);
    cameraViewLabel->setFixedSize(320, 240);
    cameraViewLabel->setStyleSheet("background-color: black;");

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
    mainLayout->addWidget(cameraViewLabel); 

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

void ControlPanel::onFrameCaptured(const cv::Mat& frame) {
    if (frame.empty()) return;
    cv::Mat rgb;
    cv::cvtColor(frame, rgb, cv::COLOR_BGR2RGB);
    QImage img((const uchar*)rgb.data, rgb.cols, rgb.rows, rgb.step, QImage::Format_RGB888);
    cameraViewLabel->setPixmap(QPixmap::fromImage(img).scaled(cameraViewLabel->size(), Qt::KeepAspectRatio));
}
