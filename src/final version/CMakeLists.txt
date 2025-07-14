cmake_minimum_required(VERSION 3.10)
project(SmartCar)

set(CMAKE_CXX_STANDARD 17)

# --------------------------------------------------
# Find required packages
# --------------------------------------------------
find_package(OpenCV REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBGPIOD REQUIRED libgpiod)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

# --------------------------------------------------
# Include directories
# --------------------------------------------------
include_directories(
    ${OpenCV_INCLUDE_DIRS}
    ${LIBGPIOD_INCLUDE_DIRS}
)

# --------------------------------------------------
# Source files
# --------------------------------------------------
set(SOURCES
    main.cpp
    controlpanel.cpp
    robot.cpp
    motor.cpp
    ultrasonic.cpp
    camera.cpp
)

set(HEADERS
    controlpanel.h
    robot.h
    motor.h
    ultrasonic.h
    camera.h
)

# --------------------------------------------------
# Create executable
# --------------------------------------------------
add_executable(smartcar ${SOURCES} ${HEADERS})

# --------------------------------------------------
# Link libraries
# --------------------------------------------------
target_link_libraries(smartcar
    ${OpenCV_LIBS}
    ${LIBGPIOD_LIBRARIES}
    Qt5::Widgets
)