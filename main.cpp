// main_gui.cpp
#include <QApplication>
#include "controlpanel.h"

int main(int argc, char *argv[]) {
    // Initialize the Qt application
    QApplication app(argc, argv);

    // Create and display the control panel window
    ControlPanel panel;
    panel.setWindowTitle("Raspberry Pi Robot Control Panel");
    panel.resize(600, 700);
    panel.show();

    // Enter the Qt event loop
    return app.exec();
}