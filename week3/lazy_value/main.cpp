#include "test_runner.h"

#include <functional>
#include <string>
#include <mutex>

using namespace std;

template<typename T>
class LazyValue {
public:
    explicit LazyValue(std::function<T()> init) : init_(init) {}

    bool HasValue() const {
        return value_.has_value();
    }

    const T &Get() const {
        if (lock_guard<mutex> guard(mutex); !value_.has_value()) {
            value_ = init_();
        }
        return value_.value();
    }

private:
    mutable std::optional<T> value_; // Guarded by mutex
    mutable mutex mutex_;
    const std::function<T()> init_;
};

void UseExample() {
    const string big_string = "Giant amounts of memory";

    LazyValue<string> lazy_string([&big_string] { return big_string; });

    ASSERT(!lazy_string.HasValue());
    ASSERT_EQUAL(lazy_string.Get(), big_string);
    ASSERT_EQUAL(lazy_string.Get(), big_string);
}

void TestInitializerIsntCalled() {
    bool called = false;

    {
        LazyValue<int> lazy_int([&called] {
            called = true;
            return 0;
        });
    }
    ASSERT(!called);
}

void DoSomething(const vector<string>& v) {
    for (auto it = v.begin(); it != v.end(); ++it) {
        auto s = *it;
    }
    string parameter;
    [parameter]() { /* do something  */ }
}
int main() {
    TestRunner tr;
    RUN_TEST(tr, UseExample);
    RUN_TEST(tr, TestInitializerIsntCalled);
    return 0;
}