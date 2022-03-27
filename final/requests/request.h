#ifndef BROWN_BELT_REQUEST_H
#define BROWN_BELT_REQUEST_H

#include <iostream>
#include <memory>

#include "bus_stop.h"
#include "route.h"
#include "parsing_stuff.h"
#include "road_map.h"


struct Request;

using RequestHolder = std::unique_ptr<Request>;

struct Request {
    enum class Type {
        ADD_ROUTE,
        ADD_STOP,
        GET_BUS_INFO,
        GET_STOP_INFO,
    };

    Request(Type type) : type(type) {}

    static RequestHolder Create(Type type);

    virtual void ParseFrom(std::string_view input) = 0;

    virtual ~Request() = default;


    const Type type;
};


struct ModifyRequest : Request {
    using Request::Request;

    virtual void Process(RoadMap &directory) const = 0;
};

template<typename ResultType>
struct ReadRequest : Request {
    using Request::Request;

    virtual std::optional<ResultType> Process(const RoadMap &directory) const = 0;
};


struct AddBusRequest : ModifyRequest {
    AddBusRequest() : ModifyRequest(Type::ADD_ROUTE) {}

    void ParseFrom(std::string_view input) override {
        auto parsed_route = ParseRoute(input);
        if (parsed_route.has_value()) {
            r = *parsed_route;
        }
    }

    void Process(RoadMap &directory) const override {
        directory.AddRoute(r);
    }

    Route r;
};


struct AddStopRequest : ModifyRequest {
    AddStopRequest() : ModifyRequest(Type::ADD_STOP) {}

    void ParseFrom(std::string_view input) override {
        std::string bus_name(ReadToken(input, ": "));
        double latitude = ConvertToDouble(std::string(ReadToken(input, ", ")));
        double longitude = ConvertToDouble(std::string(ReadToken(input, ", ")));

        stop_to_add = BusStop(bus_name, latitude, longitude);

        if (!input.empty())
            bindings_ = ParseBindings(input);
    }

    void Process(RoadMap &directory) const override {
        directory.AddStop(stop_to_add);
        for (auto& [from, to, dist] : bindings_) {
           directory.SetDirectRoadDistance(from, to, dist);
        }

    }

private:
    struct Binding {
        std::string from;
        std::string to;
        double distance;

        Binding Swap() const {
            return Binding{to, from, distance};
        }

        bool operator==(const Binding& other) const {
            return std::tie(from, to) == std::tie(other.from, other.to);
        }
    };

    struct BindingHasher {
        size_t operator()(const Binding& binding) const {
            size_t base = 37;
            size_t A = std::hash<std::string>{}(binding.from);
            size_t B = std::hash<std::string>{}(binding.to);

            return base * base * base * A +
                   base * base * B;
        }
    };


    std::vector<Binding> ParseBindings(std::string_view input) {
        std::vector<Binding> bindings;
        bindings.reserve(100);
        do {
            double dist = ConvertToDouble(ReadToken(input, "m to "));
            auto bind = Binding{stop_to_add.GetName(),std::move(std::string(ReadToken(input, ", "))), dist};
            bindings.push_back(std::move(bind));
        } while (!input.empty());

        return bindings;
}

public:
    using RoadId = double;
    BusStop stop_to_add;
    std::vector<Binding> bindings_;

};


struct GetBusInfoRequest : ReadRequest<RoadMap::RouteInfo> {
    GetBusInfoRequest() : ReadRequest<RoadMap::RouteInfo>(Type::GET_BUS_INFO) {}

    void ParseFrom(std::string_view input) override {
        bus_name = input;
    }

    std::optional<RoadMap::RouteInfo> Process(const RoadMap &directory) const override {
        return directory.GetRouteInfo(bus_name);
    }

    std::string bus_name;
};

struct GetStopInfoRequest : ReadRequest<RoadMap::StopInfo> {
    GetStopInfoRequest() : ReadRequest<RoadMap::StopInfo>(Type::GET_STOP_INFO) {}

    void ParseFrom(std::string_view input) override {
        stop_name = input;
    }

    std::optional<RoadMap::StopInfo> Process(const RoadMap &directory) const override {
        return directory.GetStopInfo(stop_name);
    }

    std::string stop_name;
};

RequestHolder Request::Create(Type type) {
    switch (type) {
        case Request::Type::ADD_ROUTE:
            return std::make_unique<AddBusRequest>();
        case Request::Type::ADD_STOP:
            return std::make_unique<AddStopRequest>();
        case Request::Type::GET_BUS_INFO:
            return std::make_unique<GetBusInfoRequest>();
        case Request::Type::GET_STOP_INFO:
            return std::make_unique<GetStopInfoRequest>();
        default:
            return nullptr;
    }
}

const std::unordered_map<std::string_view, Request::Type> STR_TO_MODIFY_REQUEST_TYPE = {
        {"Stop", Request::Type::ADD_STOP},
        {"Bus",  Request::Type::ADD_ROUTE},
};

const std::unordered_map<std::string_view, Request::Type> STR_TO_READ_REQUEST_TYPE = {
        {"Bus", Request::Type::GET_BUS_INFO},
        {"Stop", Request::Type::GET_STOP_INFO},
};

enum class RequestMode {
    MODIFY,
    READ
};

std::optional<Request::Type> ConvertRequestTypeFromString(std::string_view type_str,
                                                          RequestMode mode = RequestMode::MODIFY) {

    switch (mode) {
        case RequestMode::MODIFY: {
            if (const auto it = STR_TO_MODIFY_REQUEST_TYPE.find(type_str);
                    it != STR_TO_MODIFY_REQUEST_TYPE.end()) {
                return it->second;
            } else {
                return std::nullopt;
            }
        }
            break;

        case RequestMode::READ: {
            if (const auto it = STR_TO_READ_REQUEST_TYPE.find(type_str);
                    it != STR_TO_READ_REQUEST_TYPE.end()) {
                return it->second;
            } else {
                return std::nullopt;
            }
        }
            break;

        default:
            return std::nullopt;
    }


}

RequestHolder ParseRequest(std::string_view request_str, RequestMode mode = RequestMode::MODIFY) {
    const auto request_type = ConvertRequestTypeFromString(ReadToken(request_str), mode);
    if (!request_type) {
        return nullptr;
    }
    RequestHolder request = Request::Create(*request_type);
    if (request) {
        request->ParseFrom(request_str);
    };
    return request;
}

std::vector<RequestHolder> ReadRequests(std::istream &in_stream = std::cin, RequestMode mode = RequestMode::MODIFY) {

    int request_count;
    in_stream >> request_count;

    std::vector<RequestHolder> requests;
    requests.reserve(request_count);

    for (size_t i = 0; i < request_count; ++i) {
        std::string request_str;
        while (request_str.empty()) {
            getline(in_stream, request_str);
        }

        if (auto request = ParseRequest(request_str, mode)) {
            requests.push_back(move(request));
        }
    }

    return requests;
}


std::vector<std::string> ProcessReadRequests(std::vector<RequestHolder>&& requests, const RoadMap& dir) {
    std::vector<std::string> responses;

    for (const auto& request_holder : requests) {


        if (request_holder->type == Request::Type::GET_BUS_INFO) {
            const auto& request = static_cast<const GetBusInfoRequest&>(*request_holder);
            auto result = request.Process(dir);
            if (result.has_value()) {
                responses.push_back("Bus " + request.bus_name + ": " + result->ToString());
            } else {
                responses.push_back("Bus " + request.bus_name + ": " +"not found");
            }
        }

        if (request_holder->type == Request::Type::GET_STOP_INFO) {
            const auto& request = static_cast<const GetStopInfoRequest&>(*request_holder);
            auto result = request.Process(dir);

            if (result.has_value()) {
                if (!result->buses.empty())
                    responses.push_back("Stop " + request.stop_name + ": buses" + result->ToString());
                else
                    responses.push_back("Stop " + request.stop_name + ": no buses");
            } else {
                responses.push_back("Stop " + request.stop_name + ": not found");
            }
        }

    }

    return responses;
}

void ProcessModifyRequests(std::vector<RequestHolder>&& requests, RoadMap& dir) {

    for (const auto& request_holder : requests) {
        if (request_holder->type == Request::Type::ADD_ROUTE) {
            auto& request = static_cast<const AddBusRequest&>(*request_holder);
            request.Process(dir);
        } else if (request_holder->type == Request::Type::ADD_STOP) {
            auto& request = static_cast<const AddStopRequest&>(*request_holder);
            request.Process(dir);
        }
    }

    dir.FinishBuilding();

}

void PrintResponses(std::vector<std::string>&& responses, std::ostream& stream = std::cout) {
    for (const auto& response : responses) {
        stream << response << std::endl;
    }
}


#endif //BROWN_BELT_REQUEST_H
