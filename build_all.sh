#!/bin/bash

# Copyright 2020 Simon Hoinkis
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

set -e
WORKSPACE=$(git rev-parse --show-toplevel)
# Possible build arguments are "clean", "debug"
BUILD_ARGS=${1}

BUILD_TYPE="-DCMAKE_BUILD_TYPE=Release"

if [ "$BUILD_ARGS" = "clean" ]
then
    rm -rf node/build
    rm -rf server/build
elif [ "$BUILD_ARGS" = "debug" ]
then
    BUILD_TYPE="-DCMAKE_BUILD_TYPE=Debug"
fi

echo "~~~~~ Starting to build CoSy fIr daemon ~~~~~"
cd server
mkdir -p build
cd build
cmake $BUILD_TYPE ..
make -j8
cd $WORKSPACE

echo "~~~~~ Starting to build CoSy fIr node ~~~~~"
cd node
mkdir -p build
cd build
cmake $BUILD_TYPE -DCMAKE_TOOLCHAIN_FILE="LoRaMac-node/cmake/toolchain-arm-none-eabi.cmake" -DSENSOR_NODE=ON -DACTUATOR_NODE=OFF ..
make -j8 sensor-node sensor-node.hex
cmake $BUILD_TYPE -DCMAKE_TOOLCHAIN_FILE="LoRaMac-node/cmake/toolchain-arm-none-eabi.cmake" -DACTUATOR_NODE=ON -DSENSOR_NODE=OFF ..
make -j8 actuator-node actuator-node.hex
cd $WORKSPACE