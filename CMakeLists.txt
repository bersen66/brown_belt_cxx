cmake_minimum_required(VERSION 3.21)

project(BROWN_BELT)

set(CMAKE_CXX_STANDART 17)

include_directories(libs)

add_subdirectory(week1 week1/build)
add_subdirectory(week2 week2/build)
add_subdirectory(week3 week3/build)
add_subdirectory(week4 week4/build)
add_subdirectory(week5 week5/build)
add_subdirectory(final final/build)

