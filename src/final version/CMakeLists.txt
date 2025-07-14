cmake_minimum_required(VERSION 3.10)
project(SmartCar)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(OpenCV REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBGPIOD REQUIRED libgpiod)

include_directories(
    ${OpenCV_INCLUDE_DIRS}
    ${LIBGPIOD_INCLUDE_DIRS}
)

add_executable(smartcar
    main.cpp
    controlpanel.cpp
    robot.cpp
    motor.cpp
    ultrasonic.cpp
    camera.cpp
)

target_link_libraries(smartcar
    Qt5::Widgets
    ${OpenCV_LIBS}
    ${LIBGPIOD_LIBRARIES}
)
