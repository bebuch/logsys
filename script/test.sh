#!/bin/bash

# exit script on any error
set -e
set -o xtrace

# Build tests
cd test
mkdir -p build
cd build
cmake ..
make

# Run tests
./tests
