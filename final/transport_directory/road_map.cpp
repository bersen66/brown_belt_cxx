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
    UpdateStopStatistic(route.GetName());
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
    if (stop.GetLatitude() != 0.0 && stop.GetLongitude() != 0.0) {
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
    if (HasStop(from) && HasStop(to)) {
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
}

double RoadMap::GetGeographicalDistance(const std::string& from, const std::string& to) const {
    if (HasStop(from) && HasStop(to)) {
//
        auto &fpoint = GetStop(from);
        auto &tpoint = GetStop(to);

        return fpoint.DistanceTo(tpoint);
    }
    return 0;
}

bool RoadMap::AlreadyBinded(const std::string& from, const std::string& to) const {
    return AlreadyBinded(BusStop(from), BusStop(to));
}

void RoadMap::SetDirectRoadDistance(const std::string& from, const std::string& to, LengthType len) {
    if (!AlreadyBinded(from,to)) {
        Bind(from, to);
    }
    if (HasStop(from) && HasStop(to)) {
        auto from_id = GetStopId(from);
        auto to_id = GetStopId(to);

        auto &fpoint = GetStop(from);
        auto &tpoint = GetStop(to);


        auto edge_id = map_.GetEdgeId(from_id, to_id);
        if (edge_id.has_value()) {
            map_.SetWeight(edge_id.value(), len);
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