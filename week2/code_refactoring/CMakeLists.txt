cmake_minimum_required(VERSION 3.21)

project(CODE_REFACTORING)

set(CMAKE_CXX_STANDARD 17)

set(
        SOURCES
        main.cpp
        stats_aggregator.cpp
        stats_aggregator_test.cpp
)

set(
        HEADERS
        stats_aggregator.h
        ../../final/main.cpp)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})

