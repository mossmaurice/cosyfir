cmake_minimum_required(VERSION 3.13)
project(cosyfird VERSION 0.1.0)

find_package(Curses REQUIRED)
find_library(mosquittopp REQUIRED)
include_directories(${CURSES_INCLUDE_DIR} ${CMAKE_BINARY_DIR}/generated/ "include/")

set(CMAKE_CXX_STANDARD 17)

string(TIMESTAMP NOW "%Y-%m-%d %H:%M:%S")

configure_file(../common/version.h.in ${CMAKE_BINARY_DIR}/generated/version.h)

add_executable(cosyfird source/cosyfird/main.cpp
                        source/cosyfird/configparser.cpp
                        source/network/mqttclient.cpp
                        source/tui/tuiapp.cpp
                        source/tui/window.cpp
                        source/tui/stream.cpp
                        source/tui/status_stream.cpp
                        source/tui/message_stream.cpp)

target_link_libraries(cosyfird ${CURSES_LIBRARIES} ncurses++ mosquittopp yaml-cpp jsoncpp b64 pthread)
