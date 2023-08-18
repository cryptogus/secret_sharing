cmake_minimum_required (VERSION 3.21)

project(secret-sharing-test LANGUAGES C)

set(SOURCE
    
${CMAKE_SOURCE_DIR}/src/function1.c
${CMAKE_SOURCE_DIR}/src/function2.c
)

add_compile_options(-Wall -Wextra)

include_directories(${CMAKE_PROJECT_NAME} ${CMAKE_SOURCE_DIR}/include)

add_subdirectory(test)

add_executable(${CMAKE_PROJECT_NAME} ${SOURCE} src/main.c)

target_link_libraries(${CMAKE_PROJECT_NAME} m)