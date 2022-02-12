cmake_minimum_required(VERSION 3.21)

project(OBJECT_POOL)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} main.cpp object_pool.h)