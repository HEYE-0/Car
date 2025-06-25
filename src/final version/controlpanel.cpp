#include "controlpanel.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>
#include <chrono>

ControlPanel::ControlPanel(QWidget *parent) : QWidget(parent), robot(new Robot()), scheduler() {
    setupUI();
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
    scheduler.addTask("ultrasonic", 300, [this]() {
        auto start = std::chrono::steady_clock::now();
        this->updateUltrasonic();
        auto end = std::chrono::steady_clock::now();
        qDebug() << "[Delay] Ultrasonic took "
                 << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms";
    });

    scheduler.addTask("camera", 50, [this]() {
        auto start = std::chrono::steady_clock::now();
        this->updateCamera();
        auto end = std::chrono::steady_clock::now();
        qDebug() << "[Delay] Camera took "
                 << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " ms";
    });

    scheduler.start();
    statusLabel->setText("Status: Running");
}

void ControlPanel::onStopClicked() {
    scheduler.stop();
    robot->stopAll();
    statusLabel->setText("Status: Stopped");
}

void ControlPanel::onForwardClicked() {
    int currentSpeed = speedSlider->value();
    if (robot) robot->moveForward(currentSpeed);
    statusLabel->setText("Status: Forward");
}

void ControlPanel::onBackwardClicked() {
    int currentSpeed = speedSlider->value();
    if (robot) {
        robot->turnLeft(currentSpeed);
        robot->turnLeft(currentSpeed);
    }
    statusLabel->setText("Status: Backward");
}

void ControlPanel::onLeftClicked() {
    int currentSpeed = speedSlider->value();
    if (robot) robot->turnLeft(currentSpeed);
    statusLabel->setText("Status: Left");
}

void ControlPanel::onRightClicked() {
    int currentSpeed = speedSlider->value();
    if (robot) robot->turnRight(currentSpeed);
    statusLabel->setText("Status: Right");
}

void ControlPanel::onHaltClicked() {
    if (robot) robot->stopAll();
    statusLabel->setText("Status: Halt");
}

void ControlPanel::updateUltrasonic() {
    for (int i = 0; i < 3; ++i) {
        float dist = robot->getSensorDistance(i);
        qDebug() << "Sensor" << i << ":" << dist << "cm";
    }
}

void ControlPanel::updateCamera() {
    // placeholder for camera frame handling
    qDebug() << "Camera updated.";
}
