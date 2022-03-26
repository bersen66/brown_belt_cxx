//
// Created by ortur on 18.03.2022.
//

#ifndef BROWN_BELT_FINAL_ROUTE_H
#define BROWN_BELT_FINAL_ROUTE_H

#include <string>
#include <vector>
#include <unordered_map>
#include <string_view>




class Route {
public:
    Route() = default;

    enum class Type {
        CIRCLE,
        STRAIGHT
    };

    Route(std::string&& name, std::vector<std::string>&& stops, Type type)
        : type(type), stops_(stops), name_(name) {}

    const std::string& GetName() const {
        return name_;
    }

    Type GetType() const {
        return type;
    }

    size_t GetNumberOfStations() const {
        return stops_.size();
    }
    void SetName(const std::string& name)  {
        name_ = name;
    }

    inline auto begin() {
        return stops_.begin();
    }

    inline auto end() {
        return stops_.end();
    }

    inline const auto begin() const {
        return stops_.begin();
    }

    inline const auto end() const {
        return stops_.end();
    }

    const std::vector<std::string>& GetStations() const {
        return stops_;
    }


    friend std::ostream& operator << (std::ostream& stream, const Route& route);
private:
    Type type;
    std::vector<std::string> stops_;
    std::string name_;
};

std::optional<Route> ParseRoute(std::string_view input);

#endif //BROWN_BELT_FINAL_ROUTE_H
