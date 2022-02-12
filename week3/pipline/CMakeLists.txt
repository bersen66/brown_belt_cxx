cmake_minimum_required(VERSION 3.21)

project(PIPLINE)

set(CMAKE_CXX_STANDARD 17)

set(
        SOURCES
        main.cpp
)

set(
        HEADERS
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})