#ifndef BROWN_BELT_REQUEST_H
#define BROWN_BELT_REQUEST_H

#include <memory>

#include "bus_stop.h"
#include "route.h"
#include "parsing_stuff.h"
#include "transport_directory.h"


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

    virtual void Process(TransportDirectory &directory) const = 0;
};

template<typename ResultType>
struct ReadRequest : Request {
    using Request::Request;

    virtual std::optional<ResultType> Process(const TransportDirectory &directory) const = 0;
};


struct AddBusRequest : ModifyRequest {
    AddBusRequest() : ModifyRequest(Type::ADD_ROUTE) {}

    void ParseFrom(std::string_view input) override {
        auto parsed_route = ParseRoute(input);
        if (parsed_route.has_value()) {
            r = *parsed_route;
        }
    }

    void Process(TransportDirectory &directory) const override {
        directory.AddRoute(r);
    }

    Route r;
};


struct AddStopRequest : ModifyRequest {
    AddStopRequest() : ModifyRequest(Type::ADD_STOP) {}

    void ParseFrom(std::string_view input) override {
        auto parsed_stop = ParseBusStop(input);
        if (parsed_stop.has_value()) {
            s = *parsed_stop;
        }
    }

    void Process(TransportDirectory &directory) const override {
        directory.AddStop(s);
    }

    BusStop s;
};


struct GetBusInfoRequest : ReadRequest<TransportDirectory::BusInfo> {
    GetBusInfoRequest() : ReadRequest<TransportDirectory::BusInfo>(Type::GET_BUS_INFO) {}

    void ParseFrom(std::string_view input) override {
        bus_name = input;
    }

    std::optional<TransportDirectory::BusInfo> Process(const TransportDirectory &directory) const override {
        return directory.GetBusInfo(bus_name);
    }

    std::string bus_name;
};

struct GetStopInfoRequest : ReadRequest<TransportDirectory::StopInfo> {
    GetStopInfoRequest() : ReadRequest<TransportDirectory::StopInfo>(Type::GET_STOP_INFO) {}

    void ParseFrom(std::string_view input) override {
        stop_name = input;
    }

    std::optional<TransportDirectory::StopInfo> Process(const TransportDirectory &directory) const override {
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


std::vector<std::string> ProcessReadRequests(const std::vector<RequestHolder>& requests, const TransportDirectory& dir) {
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

void ProcessModifyRequests(const std::vector<RequestHolder>& requests, TransportDirectory& dir) {

    for (const auto& request_holder : requests) {
        if (request_holder->type == Request::Type::ADD_ROUTE) {
            auto& request = static_cast<const AddBusRequest&>(*request_holder);
            request.Process(dir);
        } else if (request_holder->type == Request::Type::ADD_STOP) {
            auto& request = static_cast<const AddStopRequest&>(*request_holder);
            request.Process(dir);
        }
    }

}

void PrintResponses(const std::vector<std::string>& responses, std::ostream& stream = std::cout) {
    for (const auto& response : responses) {
        stream << response << std::endl;
    }
}


#endif //BROWN_BELT_REQUEST_H
