#!/bin/bash

WORKSPACE=$(git rev-parse --show-toplevel)

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