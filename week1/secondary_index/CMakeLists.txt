cmake_minimum_required(VERSION 3.21)

project(DATABASE)

set(CMAKE_CXX_STANDART 17)

add_executable(
        ${PROJECT_NAME}
        main.cpp
        database.h
        database.cpp
        record.h
)



