cmake_minimum_required(VERSION 3.21)
project(RAII_BOOKING)

set(
        SOURCES
        test.cpp
)

set(
        HEADERS
        new_booking_providers.h
        new_trip_manager.h
        old_booking_providers.h
        old_trip_manager.h
        booking.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

