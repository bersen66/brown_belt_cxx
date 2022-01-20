cmake_minimum_required(VERSION 3.21)

project(SET_ITERATOR)

set(CMAKE_CXX_STANDART 17)

add_executable(${PROJECT_NAME}
        main.cpp
        node_builder.h
        node_builder.cpp

        )
