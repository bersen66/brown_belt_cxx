cmake_minimum_required(VERSION 3.21)
project(geo2d)

set(CMAKE_CXX_STANDARD 17)

set(
        HEADERS
        geo2d.h
)

set(
        SOURCES
        geo2d.cpp
)


add_library(${PROJECT_NAME} STATIC ${HEADERS} ${SOURCES})

target_include_directories(
        ${PROJECT_NAME}
        PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR}
)