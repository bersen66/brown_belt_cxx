cmake_minimum_required(VERSION 3.21)

project(TREE)

set(CMAKE_CXX_STANDARD 17)

set(
        SOURCES
        main.cpp
        Common.cpp
)

set(
        HEADERS
        Common.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})