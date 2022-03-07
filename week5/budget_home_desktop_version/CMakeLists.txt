cmake_minimum_required(VERSION 3.21)

project(BUDGET_HOME_DESKTOP)

set(CMAKE_CXX_STANDARD 17)

set(
        SOURCES
        main.cpp
		date.cpp
		query.cpp
		income_manager.cpp
)


set(
        HEADERS
        date.h
		query.h
		income_manager.h
		#solution.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

