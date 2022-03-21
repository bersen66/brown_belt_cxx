cmake_minimum_required(VERSION 3.21)

project(BUDGET_PROF_DESKTOP)

set(CMAKE_CXX_STANDARD 17)

set(
        SOURCES
        main.cpp
		date.cpp
		query.cpp
		budget_system.cpp
		income.cpp
)


set(
        HEADERS
        date.h
		query.h
		budget_system.h
		income.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

