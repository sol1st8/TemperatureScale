cmake_minimum_required(VERSION 3.11)

project(temperature_scale CXX)
set(CMAKE_CXX_STANDARD 17)

add_executable(temperature_scale main.cpp)

target_link_libraries(temperature_scale -lstdc++)
