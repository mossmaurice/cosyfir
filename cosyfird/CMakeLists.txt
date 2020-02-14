cmake_minimum_required(VERSION 3.13)
project (cosyfird)

find_package(Curses REQUIRED)
find_library(mosquittopp REQUIRED)
include_directories(${CURSES_INCLUDE_DIR} "include/")

add_executable(cosyfir-sub source/cosyfir-sub/main.cpp
                           source/network/mqttclient.cpp)
target_link_libraries(cosyfir-sub ${CURSES_LIBRARIES} mosquittopp)
