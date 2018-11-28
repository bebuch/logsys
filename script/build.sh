#!/bin/bash

# exit script on any error
set -e
set -o xtrace

PROJECT_DIR=$(pwd)
INSTALL_PATH=$PROJECT_DIR/../tmp/usr
mkdir -p $INSTALL_PATH

# Configure Project
mkdir -p $PROJECT_DIR/build
cd $PROJECT_DIR/build
cmake -DCMAKE_INSTALL_PREFIX=$INSTALL_PATH -DLOGSYS_BUILD_TESTS=ON -DLOGSYS_BUILD_EXAMPLES=ON ..

# Build tests and examples
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

# Install
make install

# Check install
git diff --no-index $INSTALL_PATH/include/logsys $PROJECT_DIR/include/logsys

CMAKE_FIND_FILE=$INSTALL_PATH/lib/cmake/logsys/logsys-config.cmake
if [ -a $CMAKE_FIND_FILE ]
then
    echo "$CMAKE_FIND_FILE - Found"
else
    echo "$CMAKE_FIND_FILE - Not found"
    false
fi

# Check install header-only usage by example project
mkdir -p $PROJECT_DIR/build-package-test-header-only
cd $PROJECT_DIR/build-package-test-header-only
cmake -DCMAKE_PREFIX_PATH=$INSTALL_PATH ../test-package/header-only
make
./test_logsys_package

# Check install lib usage by example project
mkdir -p $PROJECT_DIR/build-package-test-lib
cd $PROJECT_DIR/build-package-test-lib
cmake -DCMAKE_PREFIX_PATH=$INSTALL_PATH ../test-package/lib
make
./test_logsys_package
