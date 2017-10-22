cmake_minimum_required(VERSION 3.0)
project(jmidiexec)

set(SOURCES src/amd.cpp src/loop.cpp src/config.cpp src/application.cpp src/jacker.cpp src/drummer.cpp src/notemap.cpp src/loopmgr.cpp)


string(LENGTH "${CMAKE_SOURCE_DIR}/" SOURCE_PATH_SIZE)
add_definitions("-DSOURCE_PATH_SIZE=${SOURCE_PATH_SIZE}")

add_executable(automididrummer ${SOURCES})
target_link_libraries(automididrummer jack)

