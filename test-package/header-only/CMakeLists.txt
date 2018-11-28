cmake_minimum_required(VERSION 3.12.4)

project(test_logsys_package)

find_package(logsys REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME} test.cpp)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${logsys_INCLUDE_DIR})
