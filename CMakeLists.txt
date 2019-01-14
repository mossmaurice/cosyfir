cmake_minimum_required(VERSION 2.8.9)
project (yaair)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(yaair main.cpp)
target_link_libraries(yaair ${CURSES_LIBRARIES})
