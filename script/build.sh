#!/bin/bash

# exit script on any error
set -e
set -o xtrace

# Build liblogsys
mkdir -p build
cd build
cmake ..
make
