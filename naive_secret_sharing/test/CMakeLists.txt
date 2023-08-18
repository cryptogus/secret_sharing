include_directories(../include/)
add_executable(test ${SOURCE} test.c)
target_link_libraries(test m)