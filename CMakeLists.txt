cmake_minimum_required(VERSION 3.12.4)

# Download and unpack io_tools at configure time
configure_file(CMakeLists.txt.in io_tools-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/io_tools-download )
if(result)
  message(FATAL_ERROR "CMake step for io_tools failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/io_tools-download )
if(result)
  message(FATAL_ERROR "Build step for io_tools failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add io_tools directly to our build.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/io_tools-src
                 ${CMAKE_CURRENT_BINARY_DIR}/io_tools-build
                 EXCLUDE_FROM_ALL)


project(logsys VERSION 1.1.0 LANGUAGES CXX)

find_package(Boost REQUIRED)

set(DEFAULT_BUILD_TYPE "Release")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${DEFAULT_BUILD_TYPE}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${DEFAULT_BUILD_TYPE}" CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

include(GNUInstallDirs)

include_directories(SYSTEM ${io_tools_SOURCE_DIR}/include ${Boost_INCLUDE_DIRS})

file(GLOB SOURCE_FILES src/*.cpp)

add_library(${PROJECT_NAME} ${SOURCE_FILES})

add_subdirectory(test)
enable_testing()
add_test(NAME tests COMMAND tests)

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    PRIVATE src)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    CXX_VISIBILITY_PRESET hidden)

install(TARGETS ${PROJECT_NAME} EXPORT LogSysConfig
    ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR})
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT LogSysConfig DESTINATION share/logsys/cmake)

export(TARGETS ${PROJECT_NAME} FILE LogSysConfig.cmake)
