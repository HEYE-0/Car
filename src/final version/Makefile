# Project Makefile (Updated for Qt, OpenCV, libgpiod)
CXX = g++
CXXFLAGS = -Wall -std=c++17

QTFLAGS = `pkg-config --cflags Qt5Widgets Qt5Gui Qt5Core`
QTLIBS  = `pkg-config --libs Qt5Widgets Qt5Gui Qt5Core`

OPENCVFLAGS = `pkg-config --cflags opencv4`
OPENCVLIBS  = `pkg-config --libs opencv4`

LIBS = -lgpiod -lpthread

SRC = main.cpp controlpanel.cpp camera.cpp robot.cpp motor.cpp ultrasonic.cpp scheduler.cpp
OBJ = $(SRC:.cpp=.o)

TARGET = robot_gui

default: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) -o $@ $^ $(QTLIBS) $(OPENCVLIBS) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(QTFLAGS) $(OPENCVFLAGS) -c $< -o $@

clean:
	rm -f *.o $(TARGET)
