cmake_minimum_required(VERSION 3.21)

project(HTTP_REQUEST)

set(CMAKE_CXX_STANDARD 17)

include_directories(geo2d)

set(
        SOURCES
        main.cpp
        http_response.cpp
)

set(
        HEADERS
        http_response.h
)



add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})
