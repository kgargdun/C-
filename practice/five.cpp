#include <cstring>
#include <iostream>
#include <utility>

class Buffer {
public:
    explicit Buffer(int length)
        : data(new int[length]), len(length) {
        std::cout << "Constructor\n";
    }

    Buffer(const Buffer& other)
        : data(new int[other.len]), len(other.len) {
        std::memcpy(data, other.data, len * sizeof(int));
        std::cout << "Copy constructor\n";
    }

    Buffer& operator=(const Buffer& other) {
        std::cout << "Copy assignment\n";

        if (this == &other) {
            return *this;
        }

        int* newData = new int[other.len];
        std::memcpy(newData, other.data, other.len * sizeof(int));

        delete[] data;

        data = newData;
        len = other.len;

        return *this;
    }

    Buffer(Buffer&& other) noexcept
        : data(other.data), len(other.len) {
        other.data = nullptr;
        other.len = 0;

        std::cout << "Move constructor\n";
    }

    Buffer& operator=(Buffer&& other) noexcept {
        std::cout << "Move assignment\n";

        if (this == &other) {
            return *this;
        }

        delete[] data;

        data = other.data;
        len = other.len;

        other.data = nullptr;
        other.len = 0;

        return *this;
    }

    ~Buffer() {
        delete[] data;
        std::cout << "Destructor\n";
    }

private:
    int* data;
    int len;
};

int main() {
    Buffer a(10);

    Buffer b = a;             // copy constructor

    Buffer c(5);
    c = a;                    // copy assignment

    Buffer d = std::move(a);  // move constructor

    Buffer e(3);
    e = std::move(b);         // move assignment
}