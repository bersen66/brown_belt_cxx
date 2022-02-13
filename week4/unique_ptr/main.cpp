#include "test_runner.h"

#include <cstddef>  // нужно для nullptr_t

using namespace std;

// Реализуйте шаблон класса UniquePtr
template <typename T>
class UniquePtr {
private:
    T* data_ = nullptr;
public:
    UniquePtr() = default;
    UniquePtr(T* ptr) : data_(ptr) {}

    UniquePtr(const UniquePtr& other)  = delete;
    UniquePtr& operator = (const UniquePtr&) = delete;


    UniquePtr(UniquePtr&& other) {
        ClearData();
        MoveData(other.Get());
        other.InvalidateData();
    }



    UniquePtr& operator = (nullptr_t) {
        ClearData();
        return *this;
    }

    UniquePtr& operator = (UniquePtr&& other) {
        ClearData();
        MoveData(other.data_);
        other.InvalidateData();
        return *this;
    }

    ~UniquePtr() {
        ClearData();
    }

    T& operator * () const {
        return *data_;
    }

    T * operator -> () const {
        return data_;
    }

    T * Release() {
        T* result = data_;
        InvalidateData();
        return result;
    }

    void Reset(T * ptr) {
        if (ptr == data_)
            return;
        ClearData();
        data_ = ptr;
        //ptr = nullptr;
    }

    void Swap(UniquePtr& other) {
        std::swap(data_, other.data_);
    }

    T* Get() const {
        return data_;
    }

private:
    void MoveData(T* new_data) {
        if (new_data == data_)
            return;
        ClearData();
        data_ = new_data;
    }

    void InvalidateData() {
        data_ = nullptr;
    }

    void ClearData() {
        if (HasData())
            delete data_;
        data_ = nullptr;
    }

    bool HasData() const {
        return data_ != nullptr;
    }

};


struct Item {
    static int counter;
    int value;
    Item(int v = 0): value(v) {
        ++counter;
    }
    Item(const Item& other): value(other.value) {
        ++counter;
    }
    ~Item() {
        --counter;
    }
};

int Item::counter = 0;


void TestLifetime() {
    Item::counter = 0;
    {
        UniquePtr<Item> ptr(new Item);
        ASSERT_EQUAL(Item::counter, 1);

        ptr.Reset(new Item);
        ASSERT_EQUAL(Item::counter, 1);
    }
    ASSERT_EQUAL(Item::counter, 0);

    {
        UniquePtr<Item> ptr(new Item);
        ASSERT_EQUAL(Item::counter, 1);

        auto rawPtr = ptr.Release();
        ASSERT_EQUAL(Item::counter, 1);

        delete rawPtr;
        ASSERT_EQUAL(Item::counter, 0);
    }
    ASSERT_EQUAL(Item::counter, 0);
}

void TestGetters() {
    UniquePtr<Item> ptr(new Item(42));
    ASSERT_EQUAL(ptr.Get()->value, 42);
    ASSERT_EQUAL((*ptr).value, 42);
    ASSERT_EQUAL(ptr->value, 42);
}


void TestGet() {
    UniquePtr<Item> item_ptr(new Item(43));
    ASSERT_EQUAL(item_ptr.Get()->value, 43);

    try {
        ++(item_ptr.Get()->value);
        ASSERT("Can modify!\n");
    } catch(exception& e) {
        std::cerr << e.what() << endl;
    }
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestLifetime);
    RUN_TEST(tr, TestGetters);
    RUN_TEST(tr, TestGet);
}