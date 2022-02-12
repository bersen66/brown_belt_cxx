cmake_minimum_required(VERSION 3.21)
project(JSON_LOADER)

set(CMAKE_CXX_STANDARD 17)

set(
        SOURCES
        main.cpp
        json.cpp
)

set(
        HEADERS
        json.h
)

add_executable(
        ${PROJECT_NAME}
        ${SOURCES}
)