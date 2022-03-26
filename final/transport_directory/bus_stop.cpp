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
	auto [name, location_info] = SplitTwoStrict(input, ": ");
	if (location_info.has_value()) {
		auto location = ParseSpherePoint(*location_info);
        if (location.has_value()) {
            return std::make_optional<BusStop>(std::move(std::string(name)),
                                               location->GetLatitude(),
                                               location->GetLongitude()
            );
        } else {
            return std::nullopt;
        }
	}

    return std::nullopt;
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