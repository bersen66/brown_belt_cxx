//
// Created by ortur on 19.03.2022.
//
#include <iostream>
#include <cmath>

#include "sphere_point.h"
#include "parsing_stuff.h"

SpherePoint::SpherePoint(double latitude, double longitude)
    : latitude_(latitude), longitude_(longitude) {}


double SpherePoint::GetLatitude(SpherePoint::Format format) const {
    return format == Format::DEGREE ? latitude_ : DegreeToRadian(latitude_);
}

double SpherePoint::GetLongitude(SpherePoint::Format format) const {
    return format == Format::DEGREE ? longitude_ : DegreeToRadian(longitude_);
}

void SpherePoint::SetLatitude(double latitude) {
    latitude_ = latitude;
}

void SpherePoint::SetLongitude(double longitude) {
    longitude_ = longitude;
}

const double PI = 3.1415926535;
const double EARTH_RADIUS = 6371000.0;

double SpherePoint::DegreeToRadian(double val) const {
    return PI * val / 180;
}

double SpherePoint::DistanceTo(const SpherePoint &other) const {

    double lat_r = DegreeToRadian(latitude_);
    double lon_r = DegreeToRadian(longitude_);

    double olat_r = DegreeToRadian(other.latitude_);
    double olon_r = DegreeToRadian(other.longitude_);

    return  EARTH_RADIUS * acos(
            sin(lat_r) * sin(olat_r) +
            cos(lat_r) * cos(olat_r) *
            cos(fabs(lon_r - olon_r))
    );
}

std::optional<SpherePoint> ParseSpherePoint(std::string_view input, std::string_view delimeter) {
    auto [latitude, longitude] = SplitTwoStrict(input, delimeter);

    if (longitude.has_value()) {
        return std::make_optional<SpherePoint>(
                ConvertToDouble(latitude),
                ConvertToDouble(longitude.value())
        );
    }

    return std::nullopt;
}

std::ostream &operator<<(std::ostream &out, const SpherePoint &point) {
    out << "[" << point.GetLatitude() << ", " << point.GetLongitude() << "]";
    return out;
}

