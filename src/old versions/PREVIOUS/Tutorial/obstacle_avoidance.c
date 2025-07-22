#include <gpiod.h> 
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

using namespace std;
using namespace std::chrono_literals;

const char* chipname = "gpiochip4"; // For Raspberry Pi 5, usually gpiochip4
const int IR_LEFT_PIN = 8;  // Modify according to the actual GPIO pin number
const int IR_RIGHT_PIN = 9;

const char* SERIAL_PORT = "/dev/ttyUSB0"; // Arduino serial port
int serial_fd;

gpiod_line* setup_input(gpiod_chip* chip, int line_num) {
    gpiod_line* line = gpiod_chip_get_line(chip, line_num);
    gpiod_line_request_input(line, "obstacle_sensor");
    return line;
}

void setupSerial() {
    serial_fd = open(SERIAL_PORT, O_RDWR | O_NOCTTY | O_NDELAY);
    if (serial_fd == -1) {
        cerr << "Unable to open the serial port device" << endl;
        exit(1);
    }

    struct termios options;
    tcgetattr(serial_fd, &options);
    cfsetispeed(&options, B9600);
    cfsetospeed(&options, B9600);
    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CRTSCTS;
    tcsetattr(serial_fd, TCSANOW, &options);
}

void sendCommand(const string& cmd) {
    write(serial_fd, cmd.c_str(), cmd.length());
}

int main() {
    gpiod_chip* chip = gpiod_chip_open_by_name(chipname);
    if (!chip) {
        cerr << "Unable to open GPIO chip interface!" << endl;
        return 1;
    }

    auto leftIR = setup_input(chip, IR_LEFT_PIN);
    auto rightIR = setup_input(chip, IR_RIGHT_PIN);
    setupSerial();

    while (true) {
        int left = gpiod_line_get_value(leftIR);
        int right = gpiod_line_get_value(rightIR);

        if (left == 0 && right == 0) {
            sendCommand("STOP\n");
            cout << "Obstacles on both sides, stop" << endl;
        } else if (left == 0) {
            sendCommand("RIGHT\n");
            cout << "Left obstacle, turn right" << endl;
        } else if (right == 0) {
            sendCommand("LEFT\n");
            cout << "Right obstacle, turn left" << endl;
        } else {
            sendCommand("FORWARD\n");
            cout << "Move forward" << endl;
        }

    }

    gpiod_chip_close(chip);
    close(serial_fd);
    return 0;
}
