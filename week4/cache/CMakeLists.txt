cmake_minimum_required(VERSION 3.21)

project(CACHE)

set(CMAKE_CXX_STANDARD 17)

set(
        SOURCES
        main.cpp
        Solution.cpp
)

set(
        HEADERS
        Common.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME} pthread)