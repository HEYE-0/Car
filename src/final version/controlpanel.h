// Updated controlpanel.h
#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QSlider>
#include "robot.h"
#include "scheduler.h"

class ControlPanel : public QWidget {
    Q_OBJECT
public:
    explicit ControlPanel(QWidget *parent = nullptr);
    ~ControlPanel();

private slots:
    void onStartClicked();
    void onStopClicked();
    void onForwardClicked();
    void onBackwardClicked();
    void onLeftClicked();
    void onRightClicked();
    void onHaltClicked();

private:
    void setupUI();
    void updateUltrasonic();
    void updateCamera();

    Robot *robot;
    TaskScheduler scheduler;

    QPushButton *startButton;
    QPushButton *stopButton;
    QPushButton *forwardButton;
    QPushButton *backwardButton;
    QPushButton *leftButton;
    QPushButton *rightButton;
    QPushButton *haltButton;
    QSlider *speedSlider;
    QLabel *statusLabel;
};

#endif // CONTROLPANEL_H
