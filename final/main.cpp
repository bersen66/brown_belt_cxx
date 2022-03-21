#include <iostream>
#include <unordered_map>
#include <string>

#include "sphere_point.h"
#include "bus_stop.h"
#include "transport_directory.h"
#include "request.h"

using namespace std;
//
//void DebugStopParsing() {
//    TransportDirectory dr;
//    {
//
//        int n;
//        cin >> n;
//        for (int i = 0; i < n; i++) {
//
//            string line;
//            while (line.empty()) {
//                getline(cin, line);
//            }
//
//            auto stop = ParseBusStop(line);
//            if (stop) {
//                dr.AddStop(*stop);
//            }
//        }
//
//    }
//
//    for (const auto& [key, val] : dr) {
//        cout << val << endl;
//    }
//
//}
//
//
//void PrintState(TransportDirectory& dr) {
//
//
//    for (const auto& [key, val] : dr.stops_) {
//        cout  << key << " " << val << endl;
//    }
//
//    cout << endl;
//    cout << endl;
//
//    for (const auto& [key, val] : dr.routes_) {
//        cout << key << " " << val<< endl;
//    }
//
//    cout << endl;
//    cout << endl;
//
//    for (const auto& [key, val] : dr.stops_statistics_) {
//        cout << key << " " << val.buses_through_stop << endl;
//    }
//
//}

int main() {

    cout.precision(25);
    TransportDirectory dir;
    ProcessModifyRequests(ReadRequests(std::cin, RequestMode::MODIFY), dir);
    //PrintState(dir);
    auto responces = ProcessReadRequests(ReadRequests(std::cin, RequestMode::READ), dir);
    PrintResponses(move(responces));


    return 0;
}