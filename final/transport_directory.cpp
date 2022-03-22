//
// Created by ortur on 23.03.2022.
//

#include "transport_directory.h"

void TransportDirectory::AddStop(const BusStop& stop) {
    if (!HasStop(stop.GetName())) {
        stops_statistics_[stop.GetName()] = std::move(StopInfo());
        stops_[std::move(stop.GetName())] = stop;
    } else {
        SetStopCoordinatesIfNeeded(stop);
    }
}

void TransportDirectory::SetStopCoordinatesIfNeeded(const BusStop& stop) {
    auto& uninit_stop = stops_[stop.GetName()];

    if (uninit_stop.GetLatitude() == 0 && uninit_stop.GetLongitude() == 0) {
        uninit_stop.SetLatitude(stop.GetLatitude());
        uninit_stop.SetLongitude(stop.GetLongitude());
    }
}


void TransportDirectory::AddRoute(const Route& route) {
    routes_[route.GetName()] = route;
    AddRouteStops(route);
    UpdateStopsStatistic(route.GetName());
}

bool TransportDirectory::HasBus(const std::string& bus_name) const {
    return routes_.find(bus_name) != routes_.end();
}

bool TransportDirectory::HasStop(const std::string& stop_name) const {
    return stops_.find(stop_name) != stops_.end();
}

std::optional<TransportDirectory::BusInfo>
TransportDirectory::GetBusInfo(const std::string& bus_name) const {

    if (!HasBus(bus_name))
        return std::nullopt;

    BusInfo result;

    const Route& rt = routes_.at(bus_name);

    result.distance     = ComputeRouteLength(bus_name);
    result.stations_num = ComputeRouteSize(rt);
    result.unique_stops = ComputeUniqueStops(bus_name);

    return result;
}



size_t TransportDirectory::ComputeRouteSize(const Route& route) const {
    if (route.GetType() == Route::Type::CIRCLE) {
        return route.GetNumberOfStations();
    } else {
        return route.GetNumberOfStations() * 2 - 1;
    }
}

void TransportDirectory::AddRouteStops(const Route& route) {
    const auto& route_stations = route.GetStations();
    bool IsCircular = route.GetType() == Route::Type::CIRCLE;

    for (int i = 0; i < route_stations.size() - IsCircular; i++) {
        if (!HasStop(route_stations[i]))
            AddStop(BusStop(route_stations[i]));
    }
}

void TransportDirectory::UpdateStopsStatistic(const std::string& route_name) {
    const auto& route = routes_.at(route_name);

    const auto& route_stations = route.GetStations();
    bool IsCircular = route.GetType() == Route::Type::CIRCLE;

    for (int i = 0; i < route_stations.size() - IsCircular; i++) {
        stops_statistics_[route_stations[i]].AddBusNameToUsedSet(route_name);
    }
}

double TransportDirectory::ComputeRouteLength(const std::string& route_name) const {
    const Route& rt = routes_.at(route_name);
    const std::vector<std::string>& stations = rt.GetStations();

    double result = 0.0;
    for (int i = 0; i < stations.size() - 1; i++) {
        result += stops_.at(stations[i]).DistanceTo(stops_.at(stations[i+1]));
    }

    if (rt.GetType() == Route::Type::STRAIGHT) {
        result *= 2;
    }


    return result;
}

size_t TransportDirectory::ComputeUniqueStops(const std::string& route_name) const {
    const Route& rt = routes_.at(route_name);
    const std::vector<std::string>& stations = rt.GetStations();

    std::unordered_set<std::string_view> unique;

    for (const auto& station : stations) {
        unique.insert(station);
    }

    return unique.size();
}

std::optional<TransportDirectory::StopInfo>
TransportDirectory::GetStopInfo(const std::string& stop_name) const {
    if (HasStop(stop_name))
        return  std::make_optional<StopInfo>(stops_statistics_.at(stop_name));
    else
        return std::nullopt;
}