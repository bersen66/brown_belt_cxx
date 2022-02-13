cmake_minimum_required(VERSION 3.21)

project(TEXTURES)

set(CMAKE_CXX_STANDARD 17)

set(
        SOURCES
        main.cpp
        Solution.cpp
        Textures.cpp
)

set(
        HEADERS
        Common.h
        Textures.h
)


add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})