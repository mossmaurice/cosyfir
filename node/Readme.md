# Yet another automated irrigation system - client

This readme describes the node software of cosyfir (add link to global readme here).

## Required hardware and software

* LSN50 v1.2 from Dragino (https://wiki.dragino.com/index.php?title=Lora_Sensor_Node-LSN50)
  * STM32L072CZT6
  * Semtech SX1276
  * Battery
  * IP66 case
* FTDI UART cable
* Linux machine
  * stm32flash version 0.5 (https://sourceforge.net/p/stm32flash/wiki/Home/)

## How to build

    mkdir build
    cmake ..
    make

## How to flash

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
