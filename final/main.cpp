#include <iostream>

#include "road_map.h"
#include "request.h"
//#include "route_informator.h"

using namespace std;


int main() {
    ios_base::sync_with_stdio(false);
    cout.precision(25);
    RoadMap dir;
    //RouteInformator informator(dir);
    ProcessModifyRequests(std::move(ReadRequests(std::cin, RequestMode::MODIFY)), dir);

//    const auto& graph = dir.map_;
//    for (auto& edge : graph.edges_) {
//        std::cout << "[ " << edge->from << "->" << edge->to <<": weight = "<< edge->weight << "]" << std::endl;
//    }

    try {
       // auto responces = ProcessReadRequests(std::move(ReadRequests(std::cin, RequestMode::READ)), dir);
        PrintResponses(
                std::move(ProcessReadRequests(std::move(ReadRequests(std::cin, RequestMode::READ)), dir))
                );
    } catch(std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}