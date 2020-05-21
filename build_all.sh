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

echo "~~~~~ Start building CoSyfIr field sensor node ~~~~~"
cd node
mkdir -p build
cd build
cmake $DEBUG -DCMAKE_TOOLCHAIN_FILE="LoRaMac-node/cmake/toolchain-arm-none-eabi.cmake" -DFIELD_SENSOR=ON ..
make -j8
cd $WORKSPACE
