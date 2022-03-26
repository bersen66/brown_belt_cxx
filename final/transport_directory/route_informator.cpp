#include "road_map.h"
#include "route_informator.h"

RouteInformator::RouteInformator(const RoadMap& directory) : map_(directory){}

std::optional<RouteInfo> RouteInformator::AssembleInfo(const Route& route) const {

    if (!map_.HasRoute(route.GetName())) {
        return std::nullopt;
    }

    if (IsCahced(route)) {
        return std::make_optional<RouteInfo>(GetFromCache(route));
    }

    RouteInfo result;

    result.stations_num = ComputeStopsNum(route);
    result.unique_stops = ComputeUniqueStops(route);
    result.distance = ComputeGeographicalDistance(route);
    result.curvature = ComputeRealDistance(route) / result.distance;

    AddToCache(route, result);
    return std::make_optional<RouteInfo>(result);

}


bool RouteInformator::IsCahced(const Route& route) const {
    return cached_routes_.find(route.GetName()) != cached_routes_.end();
}

RouteInfo RouteInformator::GetFromCache(const Route& route) const {
    return cached_routes_.at(route.GetName());
}

void RouteInformator::AddToCache(const Route& route, const RouteInfo& info) const {
    cached_routes_[route.GetName()] = info;
}

size_t RouteInformator::ComputeStopsNum(const Route &route) const {
    if (route.GetType() == Route::Type::STRAIGHT)
        return 2*route.GetNumberOfStations() - 1;
    else
        return route.GetNumberOfStations();
}

size_t RouteInformator::ComputeUniqueStops(const Route &route) const {
    std::unordered_set<std::string> unique;

    for (auto& station : route.GetStations()) {
        unique.insert(station);
    }

    return unique.size();
}

double RouteInformator::ComputeGeographicalDistance(const Route &route) const {
    const auto& stations = route.GetStations();

    double result = 0.0;
    for (int i = 0; i < stations.size() - 1; i++) {
        result += map_.GetGeographicalDistance(stations[i], stations[i + 1]);
    }
    if (route.GetType() == Route::Type::STRAIGHT) {
        result *= 2;
    }

    return result;
}

double RouteInformator::ComputeRealDistance(const Route &route) const {
    const auto& stations = route.GetStations();

    double result = 0.0;

    for (int i = 0; i < stations.size() - 1; i++) {
        result += map_.GetDirectRoadLength(stations[i], stations[i + 1]);
    }

    if (route.GetType() == Route::Type::STRAIGHT) {
        result *= 2;
    }


    return result;
}