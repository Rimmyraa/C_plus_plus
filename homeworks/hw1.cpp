#include <iostream>
#include <stdexcept>

class uniquePtr {
    int* ptr = nullptr;

public:
    explicit uniquePtr(int* p = nullptr)
        : ptr(p) {}

    explicit uniquePtr(int value)
        : ptr(new int(value)) {}

    ~uniquePtr() {
        delete ptr;
    }

    uniquePtr(const uniquePtr&) = delete;
    uniquePtr& operator=(const uniquePtr&) = delete;

    uniquePtr(uniquePtr&&) = delete;
    uniquePtr& operator=(uniquePtr&&) = delete;

    bool isValid() const {
        return ptr != nullptr;
    }

    explicit operator bool() const {
        return ptr != nullptr;
    }

    int& operator*() const {
        if (!ptr)
            throw std::runtime_error("Dereferencing null uniquePtr");
        return *ptr;
    }

    int* operator->() const {
        if (!ptr)
            throw std::runtime_error("Accessing null uniquePtr");
        return ptr;
    }

    int* get() const {
        return ptr;
    }

    void reset(int* p = nullptr) {
        if (ptr != p) {
            delete ptr;
            ptr = p;
        }
    }

    int* release() {
        int* temp = ptr;
        ptr = nullptr;
        return temp;
    }
};

int main() {
    std::cout << "=== RAII uniquePtr demo ===\n";

    uniquePtr p(new int(10));

    if (p.isValid()) {
        std::cout << "p is valid\n";
    }

    if (p) {
        std::cout << "Value: " << *p << "\n";
    }

    *p = 20;
    std::cout << "New value: " << *p << "\n";

    p.reset(new int(42));
    std::cout << "After reset: " << *p << "\n";

    int* raw = p.release();
    std::cout << "After release, raw value: " << *raw << "\n";

    if (!p) {
        std::cout << "p is now empty\n";
    }

    delete raw; 

    return 0;
}

