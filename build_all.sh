#!/bin/bash

set -e
WORKSPACE=$(git rev-parse --show-toplevel)
# Possible build arguments are "clean", "debug"
BUILD_ARGS=${1}

if [ $BUILD_ARGS = "clean" ]
then
    rm -rf node/build
    rm -rf server/build
elif [ $BUILD_ARGS = "debug" ]
then
    DEBUG="-DCMAKE_BUILD_TYPE=Debug"
fi

echo "~~~~~ Start building CoSyfIr daemon ~~~~~"
cd server
mkdir -p build
cd build
cmake $DEBUG ..
make -j8
cd $WORKSPACE

echo "~~~~~ Start building CoSyfIr sensor node ~~~~~"
cd node
mkdir -p build
cd build
cmake $DEBUG -DCMAKE_TOOLCHAIN_FILE="LoRaMac-node/cmake/toolchain-arm-none-eabi.cmake" ..
make -j8
cd $WORKSPACE
