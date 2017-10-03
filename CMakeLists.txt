cmake_minimum_required(VERSION 3.0)
project(jmidiexec)

set(SOURCES amd.cpp loop.cpp config.cpp application.cpp)


string(LENGTH "${CMAKE_SOURCE_DIR}/" SOURCE_PATH_SIZE)
add_definitions("-DSOURCE_PATH_SIZE=${SOURCE_PATH_SIZE}")

add_executable(automididrummer ${SOURCES})
target_link_libraries(automididrummer jack)

