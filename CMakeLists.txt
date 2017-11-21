cmake_minimum_required(VERSION 3.0)
project(jackmididrummer)

set(SOURCES src/jmd.cpp src/loop.cpp src/config.cpp src/application.cpp src/jacker.cpp src/drummer.cpp src/notemap.cpp src/loopmgr.cpp src/ src/reactmgr.cpp src/types.cpp src/outputmap.cpp)

string(LENGTH "${CMAKE_SOURCE_DIR}/" SOURCE_PATH_SIZE)
add_definitions("-DSOURCE_PATH_SIZE=${SOURCE_PATH_SIZE}")

add_executable(jackmididrummer ${SOURCES})
target_link_libraries(jackmididrummer jack)

