//
// Created by ortur on 14.03.2022.
//

#ifndef BROWN_BELT_STOP_H
#define BROWN_BELT_STOP_H
#include <string>
#include <memory>

template<typename CoordinateType>
struct Stop {
    const std::string name;
    const CoordinateType latitude;
    const CoordinateType longitude;

    Stop(const std::string& name, const CoordinateType latitude, const CoordinateType longitude)
         : name(name), latitude(latitude), longitude(longitude) {}

};

template<typename CoordinateType>
using StopHolder = std::unique_ptr< Stop<CoordinateType> >;

template<typename CoordinateType, typename ResultType>
ResultType ComputeDistanceBetweenStops(StopHolder<CoordinateType> lhs, StopHolder<CoordinateType> rhs) {
    return 0;
}

#endif //BROWN_BELT_STOP_H
