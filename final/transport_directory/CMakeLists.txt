cmake_minimum_required(VERSION 3.21)

project(transport_directory)


include_directories(../parsers)
include_directories(../geometry)

set(
        HEADERS
        bus_stop.h
        road_map.cpp
        route.cpp
        route_informator.cpp
)

set(
        SOURCES
        bus_stop.cpp
        road_map.h
        route.h
        route_informator.h
)


add_library(${PROJECT_NAME} STATIC ${HEADERS} ${SOURCES})

target_link_libraries(${PROJECT_NAME} geometry)
target_link_libraries(${PROJECT_NAME} parsing_stuff)
