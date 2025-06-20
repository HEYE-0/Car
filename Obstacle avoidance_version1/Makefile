all: robot

robot: main.o robot.o motor.o ultrasonic.o
	g++ -o robot main.o robot.o motor.o ultrasonic.o -lgpiod -lwiringPi -std=c++11 -pthread

main.o: main.cpp robot.h
	g++ -c main.cpp

robot.o: robot.cpp robot.h motor.h ultrasonic.h
	g++ -c robot.cpp

motor.o: motor.cpp motor.h
	g++ -c motor.cpp

ultrasonic.o: ultrasonic.cpp ultrasonic.h
	g++ -c ultrasonic.cpp

clean:
	rm -f *.o robot