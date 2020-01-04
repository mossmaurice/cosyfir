cmake_minimum_required(VERSION 3.13)
project (cosyfird)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(cosyfird main.cpp)
target_link_libraries(cosyfird ${CURSES_LIBRARIES})
