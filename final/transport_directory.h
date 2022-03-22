#ifndef BROWN_BELT_TRANSPORT_DIRECTORY_H
#define BROWN_BELT_TRANSPORT_DIRECTORY_H

#include <vector>
#include <unordered_map>
#include <memory>
#include <sstream>
#include <unordered_set>
#include <set>

#include "route.h"
#include "bus_stop.h"


class TransportDirectory {
public:
    TransportDirectory() = default;
    TransportDirectory(const TransportDirectory&) = delete;

    void AddStop(const BusStop& stop);
    void AddRoute(const Route& route);

    bool HasBus(const std::string& bus_name) const;
    bool HasStop(const std::string& stop_name) const;

public:
    struct BusInfo {
        double distance;
        size_t stations_num;
        size_t unique_stops;

        std::string ToString() const {
            std::stringstream  out;
            out << stations_num << " stops on route, " <<  unique_stops << " unique stops, " << distance << " route length";
            return out.str();
        }
    };
    std::optional<BusInfo>  GetBusInfo(const std::string& bus_name) const;
private:

    void SetStopCoordinatesIfNeeded(const BusStop& stop);


    size_t ComputeRouteSize(const Route& route) const;

    void AddRouteStops(const Route& route);

    void UpdateStopsStatistic(const std::string& route_name);

    double ComputeRouteLength(const std::string& route_name) const;

    size_t ComputeUniqueStops(const std::string& route_name) const;
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

private:
    std::unordered_map<std::string, BusStop> stops_;
    std::unordered_map<std::string, StopInfo> stops_statistics_;
    std::unordered_map<std::string, Route> routes_;
};


#endif //BROWN_BELT_TRANSPORT_DIRECTORY_H
