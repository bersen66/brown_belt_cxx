#include <iostream>

#include "road_map.h"
#include "request.h"

using namespace std;


int main() {
    ios_base::sync_with_stdio(false);
    cout.precision(25);
    RoadMap dir;
    ProcessModifyRequests(ReadRequests(std::cin, RequestMode::MODIFY), dir);

    //const auto& graph = dir.map_;
    //for (auto& edge : graph.edges_) {
    //    std::cout << "[ " << edge->from << "->" << edge->to <<": weight = "<< edge->weight << "]" << std::endl;
    //}
    //try {
        auto responces = ProcessReadRequests(ReadRequests(std::cin, RequestMode::READ), dir);
        PrintResponses(move(responces), cout);
//    } catch(std::exception& e) {
//        std::cout << e.what() << std::endl;
//    }

    return 0;
}