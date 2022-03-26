cmake_minimum_required(VERSION 3.21)

project(geometry)

set(
        SOURCES
        sphere_point.cpp
)

set(
        HEADERS
        sphere_point.h
)

add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})
