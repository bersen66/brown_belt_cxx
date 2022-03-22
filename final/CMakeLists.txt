cmake_minimum_required(VERSION 3.21)

project(FINAL)
set(CMAKE_CXX_STANDARD 17)

set(
        SOURCES
        main.cpp
        route.cpp
        route.h
)

set(
        HEADERS
        sphere_point.h sphere_point.cpp parsing_stuff.h parsing_stuff.cpp bus_stop.h bus_stop.cpp transport_directory.h request.h test_transport_directory.h graph_lib/graph.h range.h transport_directory.cpp)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})