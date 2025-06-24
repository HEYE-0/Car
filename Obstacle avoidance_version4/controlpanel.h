// controlpanel.h
#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QSlider>
#include <QTimer>
#include <QImage>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPixmap>
#include <QThread>
#include <opencv2/opencv.hpp>
#include "robot.h"
#include "camera.h"

class ControlPanel : public QWidget {
    Q_OBJECT

public:
    ControlPanel(QWidget *parent = nullptr);
    ~ControlPanel();

private slots:
    void onStartClicked();
    void onStopClicked();
    void onForwardClicked();
    void onBackwardClicked();
    void onLeftClicked();
    void onRightClicked();
    void onHaltClicked();
    void updateUltrasonic();
    void updateCamera();
    void onSpeedChanged(int value);

private:
    QPushButton *btnStart, *btnStop;
    QPushButton *btnForward, *btnBackward, *btnLeft, *btnRight, *btnHalt;
    QSlider *speedSlider;
    QLabel *labelSpeed;
    QLabel *labelUltrasonic[3];
    QLabel *labelStatus;
    QLabel *labelCamera;

    QTimer *sensorTimer;
    QTimer *cameraTimer;

    Robot *robot = nullptr;
    Camera *camera = nullptr;
    int currentSpeed = 512;
};

#endif
