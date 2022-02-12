cmake_minimum_required(VERSION 3.21)

project(ZOO)

set(CMAKE_CXX_STANDARD 20)

set(    HEADERS
        animals.h
)

set(    SOURCES
        main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})