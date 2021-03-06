cmake_minimum_required(VERSION 3.12.4)

project(logsys LANGUAGES CXX)


# interface target
find_package(io_tools REQUIRED)
find_package(Boost REQUIRED)

set(DEFAULT_BUILD_TYPE "Release")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


if(MSVC)
    # Force to always compile with W4
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4"
            CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    # Update if necessary
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic -Werror")
endif()


file(GLOB SOURCE_FILES src/*.cpp)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME}
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)
target_include_directories(${PROJECT_NAME}
    SYSTEM PUBLIC $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>)
target_include_directories(${PROJECT_NAME}
    SYSTEM PUBLIC ${io_tools_INCLUDE_DIR})
target_include_directories(${PROJECT_NAME}
    SYSTEM PUBLIC ${Boost_INCLUDE_DIR})
target_link_directories(${PROJECT_NAME}
    PUBLIC $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/lib>)


# Setup package config
set(INCLUDE_INSTALL_DIR include)
set(LIB_INSTALL_DIR lib)
set(CONFIG_PACKAGE_INSTALL_DIR ${LIB_INSTALL_DIR}/cmake/${PROJECT_NAME})

include(CMakePackageConfigHelpers)
configure_package_config_file(${PROJECT_NAME}-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake
    INSTALL_DESTINATION ${LIB_INSTALL_DIR}/${PROJECT_NAME}/cmake
    PATH_VARS INCLUDE_INSTALL_DIR LIB_INSTALL_DIR)

# Install target
install(DIRECTORY include/${PROJECT_NAME}
    DESTINATION ${INCLUDE_INSTALL_DIR})
install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION lib COMPONENT libraries)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake
    DESTINATION ${LIB_INSTALL_DIR}/cmake/${PROJECT_NAME})


# other subdirectories
# only add if not inside add_subdirectory()
option(LOGSYS_BUILD_EXAMPLES "build examples" OFF)
if(${LOGSYS_BUILD_EXAMPLES})
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/examples)
endif()

option(LOGSYS_BUILD_TESTS "build tests" OFF)
if(${LOGSYS_BUILD_TESTS})
    enable_testing()
    find_package(GTest REQUIRED)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test)
    add_test(NAME tests COMMAND tests)
endif()
