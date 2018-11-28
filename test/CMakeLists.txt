project(tests)

# Add test cpp file
file(GLOB SOURCE_FILES "*.cpp")

add_executable(tests ${SOURCE_FILES})

target_link_libraries(tests logsys GTest::GTest GTest::Main)

target_include_directories(tests
    PUBLIC ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
