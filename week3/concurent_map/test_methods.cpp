#include "test_runner.h"
#include "concurrent_map.h"


int main() {
    ConcurrentMap<int, int> cm(4);
    auto access = cm.At(2);
    auto val = access.ref_to_value;
    return 0;
}