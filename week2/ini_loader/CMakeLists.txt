cmake_minimum_required(VERSION 3.21)

project(INI_LOADER)

set(CMAKE_CXX_STANDARD 17)

set(
        SOURCES
        main.cpp
        ini.cpp
)

set(
        HEADERS
        ini.h
)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})

