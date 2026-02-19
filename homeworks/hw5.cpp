#include <iostream>

class UniquePtr {
private:
    int* ptr;

public:
    UniquePtr(int* p = nullptr) {
        ptr = p;
    }

    ~UniquePtr() {
        delete ptr;
    }

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    UniquePtr(UniquePtr&& other) noexcept {
        ptr = other.ptr;
        other.ptr = nullptr;
    }

    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (this != &other) {
            delete ptr;
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }

    int& operator*() {
        return *ptr;
    }

    int* get() {
        return ptr;
    }
};


class SharedPtr {
private:
    int* ptr;
    size_t* count;

public:
    SharedPtr(int* p = nullptr) {
        ptr = p;

        if (p)
            count = new size_t(1);
        else
            count = nullptr;
    }

    ~SharedPtr() {
        release();
    }

    void release() {
        if (count) {
            (*count)--;

            if (*count == 0) {
                delete ptr;
                delete count;
            }
        }

        ptr = nullptr;
        count = nullptr;
    }


    SharedPtr(const SharedPtr& other) {
        ptr = other.ptr;
        count = other.count;

        if (count)
            (*count)++;
    }

    SharedPtr& operator=(const SharedPtr& other) {
        if (this == &other)
            return *this;

        release();

        ptr = other.ptr;
        count = other.count;

        if (count)
            (*count)++;

        return *this;
    }

    SharedPtr(SharedPtr&& other) noexcept {
        ptr = other.ptr;
        count = other.count;

        other.ptr = nullptr;
        other.count = nullptr;
    }

    SharedPtr& operator=(SharedPtr&& other) noexcept {
        if (this != &other) {
            release();

            ptr = other.ptr;
            count = other.count;

            other.ptr = nullptr;
            other.count = nullptr;
        }
        return *this;
    }

    int& operator*() {
        return *ptr;
    }

    size_t useCount() {
        if (count)
            return *count;
        return 0;
    }
};

int main() {

    std::cout << "=== UniquePtr ===\n";

    UniquePtr u1(new int(10));
    UniquePtr u2 = std::move(u1);

    if (u1.get() == nullptr)
        std::cout << "u1 is empty after move\n";

    std::cout << "u2 value: " << *u2 << "\n";


    std::cout << "\n=== SharedPtr ===\n";

    SharedPtr s1(new int(100));
    SharedPtr s2 = s1;

    std::cout << "count after copy: " << s1.useCount() << "\n";

    SharedPtr s3 = std::move(s1);

    std::cout << "count after move: " << s3.useCount() << "\n";

    return 0;
}
