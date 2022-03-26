//
// Created by ortur on 25.03.2022.
//

#pragma once
#include "oriented_graph.h"
#include "bus_stop.h"
#include "route.h"


class RoadMap {
private:
    using StopId = OrientedGraph<BusStop, double>::VertexId;
    using RoadId = OrientedGraph<BusStop, double>::EdgeId;
    using LengthType = double;
public:

    struct StopInfo{
        size_t buses_through_stop;
        std::set<std::string> buses;

        void AddBusNameToUsedSet(const std::string& bus_name) {
            buses.insert(bus_name);
            buses_through_stop = buses.size();
        }

        std::string ToString() const {
            std::stringstream out;

            for (const auto& stop_name : buses) {
                out << " " << stop_name;
            }

            return out.str();
        }
    };
    std::optional<StopInfo> GetStopInfo(const std::string& stop_name) const;

public:
    RoadMap() = default;
    RoadMap(const RoadMap&) = delete;
    const RoadMap& operator=(const RoadMap&) const = delete;

    void AddStop(const BusStop& stop);
    void AddRoute(const Route& route);

    bool HasStop(const BusStop& stop) const;
    bool HasBus(const std::string& name) const;
    bool AlreadyBinded(const BusStop& from, const BusStop& to) const;
    bool AlreadyBinded(const std::string& from, const std::string& to) const;
    bool HasRoute(const std::string& name) const;


    void SetDirectRoadDistance(const std::string& from, const std::string& to, LengthType len);


    LengthType GetDirectRoadLength(const std::string& from, const std::string& to) const;
    LengthType GetGeographicalDistance(const std::string& from, const std::string& to) const;
    const BusStop& GetStop(const std::string& name) const;
    BusStop& GetStop(const std::string& name);
    const Route& GetRoute(const std::string& name) const;


    void Bind(const BusStop& from, const BusStop& to, LengthType weight = 0);
    void FinishBuilding();

private:
    void CorrectDistancesToConnectedVertexes(const BusStop& stop);
    StopId GetStopId(const std::string& name) const;
    void InsertStop(const BusStop& stop);
    void InsertRoute(const Route& route);
    void CorrectMapCoordinates(const BusStop& stop);
    void ConnectStops(const Route& route);
    void UpdateStopStatistic(const std::string& route_name);

public:
    OrientedGraph<BusStop, double> map_;
    //std::unordered_map<std::pair<StopId, StopId>, RoadId> roads_;
    std::unordered_map<std::string, StopId> stops_;
    std::unordered_map<StopId, std::string> stop_id_to_name_;
    std::unordered_map<StopId, StopInfo> stops_statistic_;
    std::unordered_map<std::string, Route> routes_;
};


