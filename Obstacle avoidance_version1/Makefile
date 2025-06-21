all: robot

robot: main.o robot.o motor.o ultrasonic.o camera.o
	g++ -o robot main.o robot.o motor.o ultrasonic.o camera.o -lgpiod -lwiringPi `pkg-config --cflags --libs opencv4` -std=c++11 -pthread

main.o: main.cpp robot.h camera.h
	g++ -c main.cpp `pkg-config --cflags opencv4`

robot.o: robot.cpp robot.h motor.h ultrasonic.h
	g++ -c robot.cpp

motor.o: motor.cpp motor.h
	g++ -c motor.cpp

ultrasonic.o: ultrasonic.cpp ultrasonic.h
	g++ -c ultrasonic.cpp

camera.o: camera.cpp camera.h
	g++ -c camera.cpp `pkg-config --cflags opencv4`

clean:
	rm -f *.o robot
