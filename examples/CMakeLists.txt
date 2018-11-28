project(examples)

# Add test cpp file
add_executable(log_01 log_01.cpp)
target_link_libraries(log_01 logsys)

add_executable(log_02 log_02.cpp)
target_link_libraries(log_02 logsys)


add_executable(log_body_01 log_body_01.cpp)
target_link_libraries(log_body_01 logsys)

add_executable(log_body_02 log_body_02.cpp)
target_link_libraries(log_body_02 logsys)

add_executable(log_body_03 log_body_03.cpp)
target_link_libraries(log_body_03 logsys)

add_executable(log_catch_01 log_catch_01.cpp)
target_link_libraries(log_catch_01 logsys)

add_executable(log_catch_02 log_catch_02.cpp)
target_link_libraries(log_catch_02 logsys)

add_executable(log_catch_03 log_catch_03.cpp)
target_link_libraries(log_catch_03 logsys)

add_executable(log_catch_04 log_catch_04.cpp)
target_link_libraries(log_catch_04 logsys)
