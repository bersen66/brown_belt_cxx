cmake_minimum_required(VERSION 3.21)

project(concurent_map)

set(CMAKE_CXX_STANDARD 17)

set(
        SOURCES
        main.cpp
        #test_methods.cpp
)

set(
        HEADERS
       # concurrent_map.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME} pthread)