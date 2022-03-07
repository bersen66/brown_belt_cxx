cmake_minimum_required(VERSION 3.21)

project(DOMAINS)

set(CMAKE_CXX_STANDARD 17)

set(
        SOURCES
        main.cpp
)

set(
        HEADERS
        ../budget_home_desktop_version/main.cpp)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})