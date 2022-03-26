//
// Created by ortur on 25.03.2022.
//
#pragma once

#include <string>
#include <unordered_set>
#include <sstream>
#include <iomanip>
#include "route.h"
#include "road_map.h"


struct RouteInfo {
    size_t stations_num;
    size_t unique_stops;
    double distance;
    double curvature;

    std::string ToString() const {
        std::stringstream  out;
        std::setprecision(25);
        out << stations_num << " stops on route, "
            << unique_stops << " unique stops, "
            << distance << " route length, "
            << curvature << ", curvature";
        return out.str();
    }
};

class RouteInformator {
public:
    RouteInformator(const RoadMap& directory);

    std::optional<RouteInfo> AssembleInfo(const Route& route) const;
private:
    bool IsCahced(const Route& route) const;

    RouteInfo GetFromCache(const Route& route) const;

    void AddToCache(const Route& route, const RouteInfo& info) const;
private:

    size_t ComputeStopsNum(const Route &route) const;

    size_t ComputeUniqueStops(const Route &route) const;

    double ComputeGeographicalDistance(const Route &route) const;

    double ComputeRealDistance(const Route& route) const;

private:
    mutable std::unordered_map<std::string, RouteInfo> cached_routes_;
    const RoadMap& map_;
};


