//
// Created by ortur on 19.03.2022.
//

#include <iostream>
#include <optional>
#include <string>
#include <string_view>

#include "parsing_stuff.h"
#include "bus_stop.h"


BusStop::BusStop(const std::string& name, double latitude, double longitude)
    : SpherePoint(latitude, longitude), name_(name) {}

BusStop::BusStop(std::string&& name, double latitude, double longitude)
    : SpherePoint(latitude, longitude), name_(std::move(name)) {}


const std::string& BusStop::GetName() const {
	return name_;
}

void BusStop::SetName(std::string_view name) { 
	name_ = std::move(std::string(name));
} 

std::optional<BusStop> ParseBusStop(std::string_view input) {
    std::string name(ReadToken(input, ": "));
    double latitude = ConvertToDouble(ReadToken(input, ", "));
    double longitude = ConvertToDouble(ReadToken(input, ", "));
    return std::make_optional<BusStop>(name, latitude, longitude);
}

std::ostream& operator<<(std::ostream& out, const BusStop& stop) {
    out << stop.GetName() << ": " << stop.GetLatitude() << ", " << stop.GetLongitude();
    return out;
}

bool operator==(const BusStop& lhs, const BusStop& rhs) {
    return (lhs.GetName() == rhs.GetName())
            && (lhs.GetLatitude() == rhs.GetLatitude())
            && (lhs.GetLongitude() == rhs.GetLongitude());
}