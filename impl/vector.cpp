#include <cstddef>
#include <iostream>
#include <new>
#include <stdexcept>
#include <utility>

template <typename T>
class Vector {
public:
    Vector() {
        data = nullptr;
        capacity = 0;
        size = 0;
    }

    ~Vector() {
        release();
    }

    void push_back(T val) {
        if (size == capacity) {
            grow();
        }

        new (data + size) T(val);
        ++size;
    }

    T& operator[](std::size_t index) {
        if (index >= size) {
            throw std::runtime_error("Out of range");
        }

        return data[index];
    }

    // Copy constructor
    Vector(const Vector& other) {
        capacity = other.capacity;
        size = other.size;

        data = capacity == 0
            ? nullptr
            : static_cast<T*>(
                  ::operator new(capacity * sizeof(T))
              );

        for (std::size_t i = 0; i < size; ++i) {
            new (data + i) T(other.data[i]);
        }
    }

    // Copy assignment
    Vector& operator=(const Vector& other) {
        if (this == &other) {
            return *this;
        }

        release();

        capacity = other.capacity;
        size = other.size;

        data = capacity == 0
            ? nullptr
            : static_cast<T*>(
                  ::operator new(capacity * sizeof(T))
              );

        for (std::size_t i = 0; i < size; ++i) {
            new (data + i) T(other.data[i]);
        }

        return *this;
    }

    // Move constructor
    Vector(Vector&& other) {
        capacity = other.capacity;
        size = other.size;
        data = other.data;

        other.data = nullptr;
        other.size = 0;
        other.capacity = 0;
    }

    // Move assignment
    Vector& operator=(Vector&& other) {
        if (this == &other) {
            return *this;
        }

        release();

        capacity = other.capacity;
        size = other.size;
        data = other.data;

        other.data = nullptr;
        other.size = 0;
        other.capacity = 0;

        return *this;
    }

private:
    T* data;
    std::size_t capacity;
    std::size_t size;

    void grow() {
        capacity = capacity == 0 ? 1 : capacity * 2;

        T* new_data = static_cast<T*>(
            ::operator new(capacity * sizeof(T))
        );

        for (std::size_t i = 0; i < size; ++i) {
            new (new_data + i) T(data[i]);
        }

        std::swap(data, new_data);

        for (std::size_t i = 0; i < size; ++i) {
            new_data[i].~T();
        }

        ::operator delete(new_data);
    }

    void release() {
        for (std::size_t i = 0; i < size; ++i) {
            data[i].~T();
        }

        ::operator delete(data);

        data = nullptr;
        size = 0;
        capacity = 0;
    }
};

int main() {
    Vector<int> first;

    first.push_back(10);
    first.push_back(20);
    first.push_back(30);

    // Copy constructor
    Vector<int> copied(first);
    copied[0] = 100;

    std::cout << first[0] << '\n';  // 10
    std::cout << copied[0] << '\n'; // 100

    // Copy assignment
    Vector<int> copy_assigned;
    copy_assigned = first;

    std::cout << copy_assigned[1] << '\n'; // 20

    // Move constructor
    Vector<int> moved(std::move(first));

    std::cout << moved[2] << '\n'; // 30

    // Move assignment
    Vector<int> move_assigned;
    move_assigned = std::move(copied);

    std::cout << move_assigned[0] << '\n'; // 100
}