#include "route.h"
#include "parsing_stuff.h"
#include <optional>

const std::unordered_map<Route::Type, std::string> TYPE_DELIMETER = {
        {Route::Type::STRAIGHT, " - "},
        {Route::Type::CIRCLE, " > "},
};


Route::Type IdentifyRouteType(std::string_view input) {

    size_t f_pos = input.find(" - ");
    size_t c_pos = input.find(" > ");

    if (f_pos != input.npos) {
        return Route::Type::STRAIGHT;
    } else {
        return Route::Type::CIRCLE;
    }

}

std::ostream& operator << (std::ostream& stream, const Route& route) {
    bool isFirstTime = true;
    for (const auto& station : route.stops_) {
        if (isFirstTime) {
            stream << station; isFirstTime = false;
        } else {
            stream << TYPE_DELIMETER.at(route.type) << station;
        }
    }
    return stream;
}




std::optional<Route> ParseRoute(std::string_view input) {
    auto [name, stations_line] = SplitTwoStrict(input, ": ");

    if (stations_line.has_value()) {

        auto type = IdentifyRouteType(stations_line.value());

        auto stations = SplitLineIntoTokens(*stations_line,
                                                    TYPE_DELIMETER.at(type));


        return std::make_optional<Route>(
            std::move(std::string(name)),
            std::move(stations),
            type
        );

    } else {
        return std::nullopt;
    }
}
