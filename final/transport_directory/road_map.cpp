//
// Created by ortur on 25.03.2022.
//
#include <string>
#include "bus_stop.h"
#include "road_map.h"



void RoadMap::AddStop(const BusStop& stop) {
    if(!HasStop(stop)) {
        InsertStop(stop);
    } else {
        CorrectMapCoordinates(stop);
    }

}

void RoadMap::AddRoute(const Route& route) {
    InsertRoute(route);
    ConnectStops(route);
}

void RoadMap::ConnectStops(const Route& route) {
    const auto& stations = route.GetStations();

    for (size_t i = 0; i < stations.size() - 1; i++) {
        if (!HasStop(BusStop(stations[i])) ) {
            AddStop(BusStop(stations[i]));
        }
        if (!HasStop(BusStop(stations[i+1])) ) {
            AddStop(BusStop(stations[i+1]));
        }
        auto& prev_stop = GetStop(stations[i]);
        auto& next_stop = GetStop(stations[i+1]);

        if (AlreadyBinded(prev_stop, next_stop)) {
            continue;
        } else {
           Bind(prev_stop, next_stop);
        }

    }
}


bool RoadMap::AlreadyBinded(const BusStop &from, const BusStop &to) const {
    return map_.HasEdge(GetStopId(from.GetName()), GetStopId(to.GetName()));
}

void RoadMap::InsertRoute(const Route& route) {
    routes_[route.GetName()] = route;
}

void RoadMap::Bind(const BusStop& from, const BusStop& to, double weight) {

    if (!HasStop(from))
        AddStop(from);
    if (!HasStop(to))
        AddStop(to);

    auto from_id = GetStopId(from.GetName());
    auto to_id = GetStopId(to.GetName());
    map_.Bind(from_id, to_id, weight);
}

bool RoadMap::HasBus(const std::string &name) const {
    return routes_.find(name) != routes_.end();
}

bool RoadMap::HasStop(const BusStop& stop) const {
    return stops_.find(stop.GetName()) != stops_.end();
}

void RoadMap::InsertStop(const BusStop& stop) {
    std::string name = stop.GetName();
    StopId id = map_.AddVertex(MakeVertex(stop));
    stops_[name] = id;
    stop_id_to_name_[id] = name;
    stops_statistic_[id] = StopInfo{};
}

RoadMap::StopId RoadMap::GetStopId(const std::string& name) const {
    return stops_.at(name);
}

const BusStop& RoadMap::GetStop(const std::string& name) const {
    auto id = GetStopId(name);
    return (map_.GetVertex(id)->GetValue());
}

BusStop& RoadMap::GetStop(const std::string& name) {
    auto id = GetStopId(name);
    return (*map_.GetVertex(id)).GetValue();
}


void RoadMap::CorrectMapCoordinates(const BusStop& stop) {
    std::string name = stop.GetName();
    auto& stop_to_modify = GetStop(name);
    if (stop.GetLatitude() != 0.0 || stop.GetLongitude() != 0.0) {
        stop_to_modify.SetLatitude(stop.GetLatitude());
        stop_to_modify.SetLongitude(stop.GetLongitude());
    }

}

void RoadMap::CorrectDistancesToConnectedVertexes(const BusStop& stop) {
    const auto& connections = map_.GetVertexConnections(GetStopId(stop.GetName()));
    for (RoadId id : connections) {
        auto& road = map_.GetEdge(id);
        if (road->weight == 0) {
            road->weight = stop.DistanceTo(
                    GetStop(stop_id_to_name_[road->to])
            );
        }

    }
}

bool RoadMap::HasRoute(const std::string &name) const {
    return routes_.find(name) != routes_.end();
}

const Route &RoadMap::GetRoute(const std::string &name) const {
    return routes_.at(name);
}

double RoadMap::GetDirectRoadLength(const std::string &from, const std::string &to) const {
    if (HasStop(from) && HasStop(to) && map_.HasEdge(GetStopId(from), GetStopId(to))) {
        auto from_id = GetStopId(from);
        auto to_id = GetStopId(to);

        return map_.GetEdge(from_id, to_id)->weight;
    }
    return 0;
}

void RoadMap::FinishBuilding(){
    for (auto& [name_stop, id] : stops_) {
        const auto& stop = map_.GetVertex(id);
        CorrectDistancesToConnectedVertexes(stop->GetValue());
    }

    for (auto& [route_name, route] : routes_) {
        auto info = AssembleRouteInfo(route);
        if (info.has_value()) {
            route_info_[route_name] = info.value();
        }


        UpdateStopStatistic(route_name);
    }

}

double RoadMap::GetGeographicalDistance(const std::string& from, const std::string& to) const {
    if (HasStop(from) && HasStop(to)) {

        auto &fpoint = GetStop(from);
        auto &tpoint = GetStop(to);

        return fpoint.DistanceTo(tpoint);
    }
    return 0;
}

bool RoadMap::AlreadyBinded(const std::string& from, const std::string& to) const {
    return HasStop(from) && HasStop(to) && AlreadyBinded(BusStop(from), BusStop(to));
}

void RoadMap::SetDirectRoadDistance(const std::string& from, const std::string& to, LengthType len) {
    if (!AlreadyBinded(from,to)) {
        Bind(from, to);
    }
    if (HasStop(from) && HasStop(to)) {
        auto from_id = GetStopId(from);
        auto to_id = GetStopId(to);

        auto edge_id = map_.GetEdgeId(from_id, to_id);
        if (edge_id.has_value()) {
            map_.SetWeight(edge_id.value(), len);
        }

        if (!map_.HasEdge(to_id, from_id)) {
            Bind(to, from, len);
        }

    }
}


std::optional<RoadMap::StopInfo>
RoadMap::GetStopInfo(const std::string& stop_name) const {
    if (HasStop(stop_name))
        return std::make_optional<StopInfo>(stops_statistic_.at(GetStopId(stop_name)));
    else
        return std::nullopt;
}

void RoadMap::UpdateStopStatistic(const std::string& route_name) {
    const auto& route = routes_.at(route_name);

    const auto& route_stations = route.GetStations();
    bool IsCircular = route.GetType() == Route::Type::CIRCLE;

    for (int i = 0; i < route_stations.size() - IsCircular; i++) {
        stops_statistic_[GetStopId(route_stations [i])].AddBusNameToUsedSet(route_name);
    }
}

const std::unordered_map<std::string, Route>& RoadMap::GetRoutes() const {
    return routes_;
}



std::optional<RoadMap::RouteInfo> RoadMap::AssembleRouteInfo(const Route& route) const {

    if (!HasRoute(route.GetName())) {
        return std::nullopt;
    }

    RouteInfo result;

    result.stations_num = ComputeStopsNum(route);
    result.unique_stops = ComputeUniqueStops(route);


    //std::future<double> f_dist =
    result.distance = ComputeRealDistance(route);
    result.curvature = (result.distance / ComputeGeographicalDistance(route));

    return std::make_optional<RouteInfo>(result);

}

size_t RoadMap::ComputeStopsNum(const Route &route) const {
    if (route.GetType() == Route::Type::STRAIGHT)
        return 2*route.GetNumberOfStations() - 1;
    else
        return route.GetNumberOfStations();
}

size_t RoadMap::ComputeUniqueStops(const Route &route) const {
    std::unordered_set<std::string> unique;

    for (auto& station : route.GetStations()) {
        unique.insert(station);
    }

    return unique.size();
}

double RoadMap::ComputeGeographicalDistance(const Route &route) const {
    const auto& stations = route.GetStations();

    double result = 0.0;
    for (int i = 0; i < stations.size() - 1; i++) {
        result += GetGeographicalDistance(stations[i], stations[i + 1]);
    }

    if (route.GetType() == Route::Type::STRAIGHT) {
        result *= 2;
    }

    return result;
}

double RoadMap::ComputeRealDistance(const Route &route) const {
    const auto& stations = route.GetStations();

    double result = 0.0;





    for (int i = 0; i < stations.size() - 1; i++) {

        double dist = GetDirectRoadLength(stations[i], stations[i + 1]);
        if (dist == 0) {
            result += GetGeographicalDistance(stations[i], stations[i+1]);
        } else {
            result += dist;
        }
    }


    if (route.GetType() == Route::Type::STRAIGHT) {
        for (int i = stations.size() - 1; i >= 1; i--) {

            double dist = GetDirectRoadLength(stations[i], stations[i - 1]);
            if (dist == 0) {
                result += GetGeographicalDistance(stations[i], stations[i-1]);
            } else {
                result += dist;
            }
        }
    }


    return result;
}

std::optional<RoadMap::RouteInfo> RoadMap::GetRouteInfo(const std::string &route_name) const {
    if (!HasRoute(route_name)) {
        return std::nullopt;
    } else {
        return route_info_.at(route_name);
    }
}
