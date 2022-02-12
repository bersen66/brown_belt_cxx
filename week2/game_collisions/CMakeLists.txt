cmake_minimum_required(VERSION 3.21)

project(GAME_COLLISIONS)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(geo2d)
include_directories(geo2d)

set(
        SOURCES
        collide.cpp
)

set(
        HEADERS
        game_object.h
)



add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})
target_link_libraries(${PROJECT_NAME} geo2d)