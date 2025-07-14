#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QSlider>
#include "robot.h"

enum class RobotState {
    READY,
    RUNNING,
    FOLLOWING,
    OBSTACLE_AVOID,
    STOPPED
};

class ControlPanel : public QWidget, public DistanceEventInterface, public CameraEventInterface {
    Q_OBJECT
public:
    explicit ControlPanel(QWidget *parent = nullptr);
    ~ControlPanel();

    void onTooClose(float distance, int sensorId) override;
    void onMarkerDetected(int markerId, cv::Point2f pos) override;

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

    Robot *robot;
    RobotState currentState;

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
