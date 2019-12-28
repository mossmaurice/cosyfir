cmake_minimum_required(VERSION 2.8.9)
project (yaaisd)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(yaaisd main.cpp)
target_link_libraries(yaaisd ${CURSES_LIBRARIES})
