// controlpanel.cpp
#include "controlpanel.h"
#include <QApplication>
#include <QDateTime>
#include <QImage>
#include <QDebug>

ControlPanel::ControlPanel(QWidget *parent) : QWidget(parent) {
    btnStart = new QPushButton("Start");
    btnStop = new QPushButton("Exit");
    btnForward = new QPushButton("Forward");
    btnBackward = new QPushButton("Backward");
    btnLeft = new QPushButton("Left");
    btnRight = new QPushButton("Right");
    btnHalt = new QPushButton("Stop");

    speedSlider = new QSlider(Qt::Horizontal);
    speedSlider->setRange(0, 1023);
    speedSlider->setValue(512);
    labelSpeed = new QLabel("Speed: 512");

    for (int i = 0; i < 3; ++i) {
        labelUltrasonic[i] = new QLabel("Distance ");
    }
    labelStatus = new QLabel("Status: Idle");
    labelCamera = new QLabel();
    labelCamera->setFixedSize(320, 240);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QGridLayout *grid = new QGridLayout();
    grid->addWidget(btnForward, 0, 1);
    grid->addWidget(btnLeft, 1, 0);
    grid->addWidget(btnHalt, 1, 1);
    grid->addWidget(btnRight, 1, 2);
    grid->addWidget(btnBackward, 2, 1);

    QHBoxLayout *row = new QHBoxLayout();
    row->addWidget(btnStart);
    row->addWidget(btnStop);

    QHBoxLayout *infoLayout = new QHBoxLayout();
    for (int i = 0; i < 3; ++i)
        infoLayout->addWidget(labelUltrasonic[i]);
    infoLayout->addWidget(labelStatus);

    QHBoxLayout *speedLayout = new QHBoxLayout();
    speedLayout->addWidget(labelSpeed);
    speedLayout->addWidget(speedSlider);

    mainLayout->addLayout(grid);
    mainLayout->addLayout(row);
    mainLayout->addLayout(infoLayout);
    mainLayout->addLayout(speedLayout);
    mainLayout->addWidget(labelCamera);

    connect(btnStart, &QPushButton::clicked, this, &ControlPanel::onStartClicked);
    connect(btnStop, &QPushButton::clicked, this, &ControlPanel::onStopClicked);
    connect(btnForward, &QPushButton::clicked, this, &ControlPanel::onForwardClicked);
    connect(btnBackward, &QPushButton::clicked, this, &ControlPanel::onBackwardClicked);
    connect(btnLeft, &QPushButton::clicked, this, &ControlPanel::onLeftClicked);
    connect(btnRight, &QPushButton::clicked, this, &ControlPanel::onRightClicked);
    connect(btnHalt, &QPushButton::clicked, this, &ControlPanel::onHaltClicked);
    connect(speedSlider, &QSlider::valueChanged, this, &ControlPanel::onSpeedChanged);

    sensorTimer = new QTimer(this);
    cameraTimer = new QTimer(this);
    connect(sensorTimer, &QTimer::timeout, this, &ControlPanel::updateUltrasonic);
    connect(cameraTimer, &QTimer::timeout, this, &ControlPanel::updateCamera);
}

ControlPanel::~ControlPanel() {
    delete robot;
    delete camera;
}

void ControlPanel::onStartClicked() {
    if (!robot) robot = new Robot();
    if (!camera) camera = new Camera();
    labelStatus->setText("Status: Running");
    sensorTimer->start(300);
    cameraTimer->start(50);
}

void ControlPanel::onStopClicked() {
    if (robot) {
        robot->stopAll();
        delete robot;
        robot = nullptr;
    }
    if (camera) {
        delete camera;
        camera = nullptr;
    }
    labelStatus->setText("Status: Stopped");
    sensorTimer->stop();
    cameraTimer->stop();
    QApplication::quit();
}

void ControlPanel::onForwardClicked() {
    if (robot) robot->moveForward(currentSpeed);
    labelStatus->setText("Status: Forward");
}

void ControlPanel::onBackwardClicked() {
    if (robot) {
        robot->turnLeft(currentSpeed);
        robot->turnLeft(currentSpeed);
        labelStatus->setText("Status: Backward");
    }
}

void ControlPanel::onLeftClicked() {
    if (robot) robot->turnLeft(currentSpeed);
    labelStatus->setText("Status: Left");
}

void ControlPanel::onRightClicked() {
    if (robot) robot->turnRight(currentSpeed);
    labelStatus->setText("Status: Right");
}

void ControlPanel::onHaltClicked() {
    if (robot) robot->stopAll();
    labelStatus->setText("Status: Halt");
}

void ControlPanel::updateUltrasonic() {
    if (!robot) return;
    for (int i = 0; i < 3; ++i) {
        float dist = robot->getSensorDistance(i);
        labelUltrasonic[i]->setText(QString("Sensor %1: %2 cm").arg(i).arg(dist, 0, 'f', 1));
    }
}

void ControlPanel::updateCamera() {
    if (!camera) return;
    cv::Mat frame;
    if (camera->readFrame(frame)) {
        cv::resize(frame, frame, cv::Size(320, 240));
        QImage img(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_BGR888);
        labelCamera->setPixmap(QPixmap::fromImage(img));
    }
}

void ControlPanel::onSpeedChanged(int value) {
    currentSpeed = value;
    labelSpeed->setText(QString("Speed: %1").arg(value));
}
