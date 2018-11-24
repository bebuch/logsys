#!/bin/bash

# exit script on any error
set -e
set -o xtrace

# Build liblogsys and tests
mkdir -p build
cd build
cmake ..
make

# Run tests
./test/tests
