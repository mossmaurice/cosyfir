#!/bin/bash

set -e
WORKSPACE=$(git rev-parse --show-toplevel)
# Possible build arguments are "clean", "test"
BUILD_ARGS=${1}

if [ $BUILD_ARGS = "clean" ]
then
    rm -rf node/build
    rm -rf cosyfird/build
fi

echo "~~~~~ Start building CoSyfIr daemon ~~~~~"
cd cosyfird
mkdir -p build
cd build
cmake ..
make -j8
cd $WORKSPACE

echo "~~~~~ Start building CoSyfIr node ~~~~~"
cd node
mkdir -p build
cd build
cmake ..
make -j8
cd $WORKSPACE

#if [$BUILD_ARGS = "test"]
#then
    #run tests
#fi
