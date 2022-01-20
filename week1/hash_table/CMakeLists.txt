cmake_minimum_required(VERSION 3.21)

project(HASH_TABLE)

set(CMAKE_CXX_STANDART 17)

add_executable(${PROJECT_NAME}
        main.cpp
        hash_table.h
        )
