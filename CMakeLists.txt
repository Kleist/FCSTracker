cmake_minimum_required(VERSION 2.8)
project(HelloOpenCV)

find_package(OpenCV REQUIRED)

file(GLOB files *.cpp *.h)
add_definitions(-std=c++11 -Wall -pedantic)
add_executable(helloopencv ${files})
target_link_libraries(helloopencv ${OpenCV_LIBS})
