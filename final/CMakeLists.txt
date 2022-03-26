cmake_minimum_required(VERSION 3.21)

project(FINAL)
set(CMAKE_CXX_STANDARD 17)


include_directories(requests)
include_directories(graphs)

add_subdirectory(transport_directory)
include_directories(transport_directory)

add_subdirectory(parsers)
include_directories(parsers)

add_subdirectory(geometry)
include_directories(geometry)

set(
        SOURCES
        main.cpp
)

set(
        HEADERS
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME} transport_directory)
target_link_libraries(${PROJECT_NAME} parsing_stuff)
target_link_libraries(${PROJECT_NAME} geometry)