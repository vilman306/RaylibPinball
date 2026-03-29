cmake_minimum_required(VERSION 3.16)
project(RaylibPinball)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(thirdParty/raylib)

file(GLOB SOURCES src/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE include)
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

target_compile_definitions(RaylibPinball PRIVATE RAYMATH_DISABLE_CPP_OPERATORS) # to avoid conflict between raymath.h and Vec2Extensions.h
