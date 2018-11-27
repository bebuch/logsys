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
test/tests

# Run examples
examples/log_01
examples/log_02
examples/log_body_01
examples/log_body_02
examples/log_body_03
examples/log_catch_01
examples/log_catch_02
examples/log_catch_03
examples/log_catch_04
