cmake_minimum_required(VERSION 3.12)
project(XML_LOADER)

set(CMAKE_CXX_STANDARD 17)

add_executable(
        ${PROJECT_NAME}
        main.cpp
        xml.h
        xml.cpp
)