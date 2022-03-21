//
// Created by ortur on 19.03.2022.
//

#ifndef BROWN_BELT_SPHERE_POINT_H
#define BROWN_BELT_SPHERE_POINT_H

#include <string_view>
#include <optional>

class SpherePoint {
public:
    enum class Format {
        DEGREE,
        RADIAN,
    };

    explicit SpherePoint(double latitude = 0, double longitude = 0);

    double DistanceTo(const SpherePoint& other) const;

    double GetLatitude(Format fmt = Format::DEGREE) const;
    double GetLongitude(Format fmt = Format::DEGREE) const;

    void SetLatitude(double latitude);
    void SetLongitude(double longitude);

    virtual ~SpherePoint() = default;

private:
    double DegreeToRadian(double val) const;
protected:
    double latitude_;
    double longitude_;
};

std::optional<SpherePoint> ParseSpherePoint(std::string_view input, std::string_view delimeter = ", ");

std::ostream& operator<<(std::ostream& out, const SpherePoint& point);
#endif //BROWN_BELT_SPHERE_POINT_H
