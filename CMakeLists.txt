cmake_minimum_required(VERSION 3.10)
project(opencv_code)

find_package(OpenCV REQUIRED)
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
include_directories(${YOUR_DIRECTORY})

file(GLOB_RECURSE SOURCES src/*.cpp)

add_executable(main ${SOURCES})

target_link_libraries(main ${OpenCV_LIBS} sfml-graphics sfml-window sfml-system)
target_include_directories(main PUBLIC ${OpenCV_INCLUDE_DIRS})