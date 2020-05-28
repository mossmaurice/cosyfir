##
##   ______                              _
##  / _____)             _              | |
## ( (____  _____ ____ _| |_ _____  ____| |__
##  \____ \| ___ |    (_   _) ___ |/ ___)  _ \
##  _____) ) ____| | | || |_| ____( (___| | | |
## (______/|_____)_|_|_| \__)_____)\____)_| |_|
## (C)2013-2017 Semtech
##  ___ _____ _   ___ _  _____ ___  ___  ___ ___
## / __|_   _/_\ / __| |/ / __/ _ \| _ \/ __| __|
## \__ \ | |/ _ \ (__| ' <| _| (_) |   / (__| _|
## |___/ |_/_/ \_\___|_|\_\_| \___/|_|_\\___|___|
## embedded.connectivity.solutions.==============
##
## License:  Revised BSD License, see LICENSE.TXT file included in the project
## Authors:  Johannes Bruder (STACKFORCE), Miguel Luis (Semtech), Simon Hoinkis
##
cmake_minimum_required(VERSION 3.13)
project (cosyfir-node VERSION 0.1.0)

option(SENSOR_NODE "Create sensor-node, which can publish sensor uplink messages from the field" OFF)
option(ACTUATOR_NODE "Create actuator-node, which can subscribe to irrigation downlink messages" OFF)

if (${SENSOR_NODE} AND ${ACTUATOR_NODE})
    message(FATAL_ERROR "One devEUI for two devices is not possible. Either enable SENSOR_NODE or ACTUATOR_NODE!")
endif()

# Configure LSN50/stm32l072xx-specific things
set(BOARD LSN50)
set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/LoRaMac-node/src/boards/LSN50/cmsis/arm-gcc/stm32l072xx_flash.ld)
include(LoRaMac-node/cmake/stm32l0.cmake)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/LoRaMac-node/src/boards/LSN50)
set(RADIO sx1276 CACHE INTERNAL "Radio sx1276 selected")

# Binutils helper
include(LoRaMac-node/cmake/binutils-arm-none-eabi.cmake)

# General stuff
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/LoRaMac-node/src/boards)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/LoRaMac-node/src/radio)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/LoRaMac-node/src/system)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/LoRaMac-node/src/mac)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/LoRaMac-node/src/peripherals)

# Create a lib for common parts
add_library(common
    common/LoRa.c
    LoRaMac-node/src/apps/LoRaMac/common/NvmCtxMgmt.c
    $<TARGET_OBJECTS:mac>
    $<TARGET_OBJECTS:system>
    $<TARGET_OBJECTS:radio>
    $<TARGET_OBJECTS:peripherals>
    $<TARGET_OBJECTS:LSN50>
)

target_compile_definitions(common
    PRIVATE
    ACTIVE_REGION=LORAMAC_REGION_EU868
    PUBLIC
    $<BUILD_INTERFACE:$<TARGET_PROPERTY:mac,INTERFACE_COMPILE_DEFINITIONS>>
)

target_include_directories(common
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/common/
    ${CMAKE_CURRENT_SOURCE_DIR}/LoRaMac-node/src/apps/LoRaMac/common/
    $<BUILD_INTERFACE:$<TARGET_PROPERTY:mac,INTERFACE_INCLUDE_DIRECTORIES>>
    $<BUILD_INTERFACE:$<TARGET_PROPERTY:system,INTERFACE_INCLUDE_DIRECTORIES>>
    $<BUILD_INTERFACE:$<TARGET_PROPERTY:radio,INTERFACE_INCLUDE_DIRECTORIES>>
    $<BUILD_INTERFACE:$<TARGET_PROPERTY:peripherals,INTERFACE_INCLUDE_DIRECTORIES>>
    $<BUILD_INTERFACE:$<TARGET_PROPERTY:LSN50,INTERFACE_INCLUDE_DIRECTORIES>>
)

target_link_libraries(common
    m
)

if(${SENSOR_NODE})
    add_executable(sensor-node
        sensor/main.c
        sensor/ds18b20.c
    )

    target_link_libraries(sensor-node
        common
    )

    create_hex_output(sensor-node)
endif()


if(${ACTUATOR_NODE})
    add_executable(actuator-node
        actuator/main.c
    )

    target_link_libraries(actuator-node
        common
    )

    create_hex_output(actuator-node)
endif()
