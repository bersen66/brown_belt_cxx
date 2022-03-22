#include <iostream>

#include "transport_directory.h"
#include "request.h"

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cout.precision(25);
    TransportDirectory dir;
    ProcessModifyRequests(ReadRequests(std::cin, RequestMode::MODIFY), dir);
    auto responces = ProcessReadRequests(ReadRequests(std::cin, RequestMode::READ), dir);
    PrintResponses(move(responces));
    return 0;
}