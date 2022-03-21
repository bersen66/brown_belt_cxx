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

    void AddStop(const BusStop& stop) {
        if (!HasStop(stop.GetName())) {
            stops_statistics_[stop.GetName()] = std::move(StopInfo());
            stops_[std::move(stop.GetName())] = stop;
        } else {
            SetCoordinatesIfNeeded(stop);
        }

    }

    void SetCoordinatesIfNeeded(const BusStop& stop) {
        auto& uninit_stop = stops_[stop.GetName()];

        if (uninit_stop.GetLatitude() == 0 && uninit_stop.GetLongitude() == 0) {
            uninit_stop.SetLatitude(stop.GetLatitude());
            uninit_stop.SetLongitude(stop.GetLongitude());
        }
    }

    void AddRoute(const Route& route) {
        routes_[route.GetName()] = route;
        AddRouteStops(route);
        UpdateStopsStatistic(route.GetName());
    }

    bool HasBus(const std::string& bus_name) const {
        return routes_.find(bus_name) != routes_.end();
    }


    bool HasStop(const std::string& stop_name) const {
        return stops_.find(stop_name) != stops_.end();
    }

    inline auto begin() {
        return stops_.begin();
    }

    inline auto end() {
        return stops_.end();
    }

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

    std::optional<BusInfo>  GetBusInfo(const std::string& bus_name) const {
        if (!HasBus(bus_name))
            return std::nullopt;

        BusInfo result;

        const Route& rt = routes_.at(bus_name);

        result.distance = ComputeRouteLength(bus_name);

        result.stations_num = ComputeRouteSize(rt);
        result.unique_stops = ComputeUniqueStops(bus_name);

        return result;
    }
private:

    size_t ComputeRouteSize(const Route& route) const {
        if (route.GetType() == Route::Type::CIRCLE) {
            return route.GetNumberOfStations();
        } else {
            return route.GetNumberOfStations() * 2 - 1;
        }
    }

    void AddRouteStops(const Route& route) {
        const auto& route_stations = route.GetStations();
        bool IsCircular = route.GetType() == Route::Type::CIRCLE;

        for (int i = 0; i < route_stations.size() - IsCircular; i++) {
            if (!HasStop(route_stations[i]))
                AddStop(BusStop(route_stations[i]));
        }
    }

    void UpdateStopsStatistic(const std::string& route_name) {
        const auto& route = routes_.at(route_name);

        const auto& route_stations = route.GetStations();
        bool IsCircular = route.GetType() == Route::Type::CIRCLE;

        for (int i = 0; i < route_stations.size() - IsCircular; i++) {
            stops_statistics_[route_stations[i]].AddBusNameToUsedSet(route_name);
//            if (HasStop(route_stations[i]))
//                stops_statistics_.at(route_stations[i]).buses_through_stop++;
        }
    }

    double ComputeRouteLength(const std::string& route_name) const {
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

    size_t ComputeUniqueStops(const std::string& route_name) const {
        const Route& rt = routes_.at(route_name);

        const std::vector<std::string>& stations = rt.GetStations();



        std::unordered_set<std::string_view> unique;

        for (const auto& station : stations) {
             unique.insert(station);
        }

        return unique.size();
    }
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

    std::optional<StopInfo> GetStopInfo(const std::string& stop_name) const {
        if (HasStop(stop_name))
            return  std::make_optional<StopInfo>(stops_statistics_.at(stop_name));
        else
            return std::nullopt;
    }

private:
    std::unordered_map<std::string, BusStop> stops_;
    std::unordered_map<std::string, StopInfo> stops_statistics_;
    std::unordered_map<std::string, Route> routes_;
};


#endif //BROWN_BELT_TRANSPORT_DIRECTORY_H
