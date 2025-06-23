// ===================== Makefile =====================
QTFLAGS = `pkg-config --cflags --libs Qt5Widgets Qt5Gui Qt5Core`
OPENCVFLAGS = `pkg-config --cflags --libs opencv4`

all: robot_gui test_robot

robot_gui: main_gui.o controlpanel.o robot.o motor.o ultrasonic.o camera.o
	g++ -o robot_gui main_gui.o controlpanel.o robot.o motor.o ultrasonic.o camera.o -lgpiod -lwiringPi -pthread $(QTFLAGS) $(OPENCVFLAGS) -std=c++11

test_robot: test_robot.o robot.o motor.o ultrasonic.o
	g++ -o test_robot test_robot.o robot.o motor.o ultrasonic.o -lgpiod -lwiringPi -std=c++11

main_gui.o: main_gui.cpp controlpanel.h
	g++ -c main_gui.cpp $(QTFLAGS)

controlpanel.o: controlpanel.cpp controlpanel.h robot.h camera.h
	g++ -c controlpanel.cpp $(QTFLAGS) $(OPENCVFLAGS)

robot.o: robot.cpp robot.h motor.h ultrasonic.h
	g++ -c robot.cpp

motor.o: motor.cpp motor.h
	g++ -c motor.cpp

ultrasonic.o: ultrasonic.cpp ultrasonic.h
	g++ -c ultrasonic.cpp

camera.o: camera.cpp camera.h
	g++ -c camera.cpp $(OPENCVFLAGS)

test_robot.o: test_robot.cpp robot.h motor.h ultrasonic.h
	g++ -c test_robot.cpp

clean:
	rm -f *.o robot_gui test_robot