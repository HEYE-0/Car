cmake_minimum_required(VERSION 3.10)
project(car_control_ws)

set(CMAKE_CXX_STANDARD 11)

find_package(Boost REQUIRED COMPONENTS system thread)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(car_control_ws main.cpp)
target_link_libraries(car_control_ws ${Boost_LIBRARIES})
