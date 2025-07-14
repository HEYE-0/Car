# Updated Makefile with moc support for Qt class
CXX = g++
CXXFLAGS = -Wall -std=c++17

QTFLAGS = `pkg-config --cflags Qt5Widgets Qt5Gui Qt5Core`
QTLIBS  = `pkg-config --libs Qt5Widgets Qt5Gui Qt5Core`

OPENCVFLAGS = `pkg-config --cflags opencv4`
OPENCVLIBS  = `pkg-config --libs opencv4`

LIBS = -lgpiod -lpthread

SRC = main.cpp controlpanel.cpp camera.cpp robot.cpp motor.cpp ultrasonic.cpp scheduler.cpp moc_controlpanel.cpp
OBJ = $(SRC:.cpp=.o)

TARGET = robot_gui

default: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) -o $@ $^ $(QTLIBS) $(OPENCVLIBS) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(QTFLAGS) $(OPENCVFLAGS) -c $< -o $@

moc_controlpanel.cpp: controlpanel.h
	moc controlpanel.h -o moc_controlpanel.cpp

clean:
	rm -f *.o *.cpp~ $(TARGET) moc_controlpanel.cpp
