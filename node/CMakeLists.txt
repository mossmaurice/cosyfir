# @todo diff
cmake_minimum_required(VERSION 3.13)
project (cosyfir-node VERSION 0.1.0)

option(FIELD_SENSOR "Create cosyfir-node, which can publish sensor uplink messages" ON)
option(IRRIGATION_ACTUATOR "Create cosyfir-node, which can subscribe to irrigation downlink messages" OFF)

if (${FIELD_SENSOR} AND ${IRRIGATION_ACTUATOR})
    message(FATAL_ERROR "Either enable FIELD_SENSOR or IRRIGATION_ACTUATOR!")
endif()

# Configure stm32l072xx-specific things
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

# Add custom app
add_executable(cosyfir-node main.c
                            LoRaMac-node/src/apps/LoRaMac/common/NvmCtxMgmt.c
                            $<TARGET_OBJECTS:mac>
                            $<TARGET_OBJECTS:system>
                            $<TARGET_OBJECTS:radio>
                            $<TARGET_OBJECTS:peripherals>
                            $<TARGET_OBJECTS:LSN50>
)

target_compile_definitions(cosyfir-node PRIVATE ACTIVE_REGION=LORAMAC_REGION_EU868)

target_compile_definitions(cosyfir-node  PUBLIC
    $<BUILD_INTERFACE:$<TARGET_PROPERTY:mac,INTERFACE_COMPILE_DEFINITIONS>>
)

target_include_directories(cosyfir-node PUBLIC
    $<BUILD_INTERFACE:$<TARGET_PROPERTY:mac,INTERFACE_INCLUDE_DIRECTORIES>>
    $<BUILD_INTERFACE:$<TARGET_PROPERTY:system,INTERFACE_INCLUDE_DIRECTORIES>>
    $<BUILD_INTERFACE:$<TARGET_PROPERTY:radio,INTERFACE_INCLUDE_DIRECTORIES>>
    $<BUILD_INTERFACE:$<TARGET_PROPERTY:peripherals,INTERFACE_INCLUDE_DIRECTORIES>>
    $<BUILD_INTERFACE:$<TARGET_PROPERTY:LSN50,INTERFACE_INCLUDE_DIRECTORIES>>
)

target_link_libraries(cosyfir-node PRIVATE m)

create_hex_output(cosyfir-node)
