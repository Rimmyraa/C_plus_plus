#include <iostream>

class uniquePtr {
    int* ptr;

public:
    uniquePtr(int* p = nullptr) {
        ptr = p;
    }

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

    operator bool() const {
        return ptr != nullptr;
    }

    int& operator*() const {
        return *ptr;
    }

    int* operator->() const {
        return ptr;
    }

    int* get() const {
        return ptr;
    }

    void reset(int* p = nullptr) {
        delete ptr;
        ptr = p;
    }

    int* release() {
        int* temp = ptr;
        ptr = nullptr;
        return temp;
    }
};

int main() {

    uniquePtr p(new int(10));

    if (p) {
        std::cout << "Value: " << *p << std::endl;
    }

    *p = 25;
    std::cout << "Changed value: " << *p << std::endl;

    p.reset(new int(50));
    std::cout << "After reset: " << *p << std::endl;

    int* raw = p.release();
    std::cout << "Released value: " << *raw << std::endl;

    delete raw;

    if (!p) {
        std::cout << "Pointer is now empty" << std::endl;
    }

    return 0;
}
