# COntrol SYstem For IRigation

Cosyfir helps you watering your plants. The node software runs on an STM32 ARM Cortex microcontroller. The server software runs on UNIX-based system like Debian. The following picture depicts an example setup:

     []        +--+       +--+         \  /        \o/
     []  <->   |  |  <->  |  |  <->   ( )( )  <->   |
     \/  LoRa  +--+       +--+  MQTT   (  )   SSH  / \

    Sensor    LoRaWAN     TTN          RPi        User
     Node     Gateway    Server       @home
             (Not part of cosyfir)

This readme describes both the server (e.g. Raspberry Pi) and the sensor node software in the following sections.

## Prerequisites

E.g. on a Debian based system install the following packages:

    sudo apt install clang-format stm32flash arm-none-eabi-gcc gcc cmake

## Build everything

    ./build_all.sh

If you want to run the tests after the build add "test".

    ./build_all.sh test

For a clean build add the parameter "clean" instead of "test".

## Server

tbd

## Node

This section describes the node software which communicates based on [LoRa](https://en.wikipedia.org/wiki/LoRa) and [TTN](https://www.thethingsnetwork.org/).

### Required hardware and software

* [LSN50 v1.2](https://wiki.dragino.com/index.php?title=Lora_Sensor_Node-LSN50) from Dragino
  * STM32L072CZT6
  * Semtech SX1276
  * Battery
  * IP66 case
* FTDI UART cable
* Linux machine
  * [stm32flash](https://sourceforge.net/p/stm32flash/wiki/Home/) version  >=0.5

### How to flash

Connected as described below:

    GND (black):  JP4 Pin11 GND
    TXD (orange): JP3 Pin9 PA3 (USART1)
    RXD (yellow): JP3 Pin10 PA2 (USART1)

Find out where your usbserial device is mounted:

    cd ..
    journalctl -k -n 100

And then do:

    sudo stm32flash -w cosyfir-node /dev/ttyUSBx

## Todo

* cmake -DCMAKE_TOOLCHAIN_FILE="cmake/toolchain-arm-none-eabi.cmake" ..
* Add CMake flag for support of different sensors (e.g. temperature, soil moisture)
* Add LoRa stack as submodule
  * Add variable in CMake for TTN credentials?
