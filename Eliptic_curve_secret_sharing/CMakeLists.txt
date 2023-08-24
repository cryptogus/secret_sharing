cmake_minimum_required (VERSION 3.21)

project(Eliptic-secret-sharing LANGUAGES C)

# OpenSSL 경로 설정
find_package(OpenSSL REQUIRED)

set(SOURCE
    
    ${CMAKE_SOURCE_DIR}/src/ecc.c
    ${CMAKE_SOURCE_DIR}/src/poly.c
    ${CMAKE_SOURCE_DIR}/src/recov.c
)

add_compile_options(-Wall -Wextra)

include_directories(${CMAKE_PROJECT_NAME} ${CMAKE_SOURCE_DIR}/include)

#add_subdirectory(test)

add_executable(${CMAKE_PROJECT_NAME} ${SOURCE} src/main.c)

target_link_libraries(${CMAKE_PROJECT_NAME} OpenSSL::Crypto)