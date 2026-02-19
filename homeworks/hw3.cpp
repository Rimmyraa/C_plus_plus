#include <iostream>

class SharedPtrInt {
private:
    int* ptr;
    size_t* count;

public:
    SharedPtrInt() {
        ptr = nullptr;
        count = nullptr;
    }


    SharedPtrInt(int* p) {
        ptr = p;
        if (p) {
            count = new size_t(1);
        } else {
            count = nullptr;
        }
    }

 
    SharedPtrInt(int value) {
        ptr = new int(value);
        count = new size_t(1);
    }


    ~SharedPtrInt() {
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

    SharedPtrInt(const SharedPtrInt& other) {
        ptr = other.ptr;
        count = other.count;

        if (count) {
            (*count)++;
        }
    }

    SharedPtrInt& operator=(const SharedPtrInt& other) {
        if (this == &other)
            return *this;

        release();

        ptr = other.ptr;
        count = other.count;

        if (count) {
            (*count)++;
        }

        return *this;
    }


    SharedPtrInt(SharedPtrInt&&) = delete;
    SharedPtrInt& operator=(SharedPtrInt&&) = delete;


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

  
    size_t useCount() const {
        if (count)
            return *count;
        return 0;
    }
};

int main() {

    std::cout << "Создаем p1\n";
    SharedPtrInt p1(new int(10));
    std::cout << "p1 value: " << *p1 << std::endl;
    std::cout << "p1 count: " << p1.useCount() << std::endl;

    std::cout << "\nКопируем p1 в p2\n";
    SharedPtrInt p2 = p1;

    std::cout << "p1 count: " << p1.useCount() << std::endl;
    std::cout << "p2 count: " << p2.useCount() << std::endl;

    std::cout << "\nМеняем значение через p2\n";
    *p2 = 50;

    std::cout << "p1 value: " << *p1 << std::endl;
    std::cout << "p2 value: " << *p2 << std::endl;

    std::cout << "\nВыход из main...\n";

    return 0;
}
