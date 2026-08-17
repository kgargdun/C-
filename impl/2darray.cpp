#include <cstddef>
#include <limits>
#include <memory>
#include <stdexcept>

template <typename T>
class Contiguous2DArray {
private:
    std::size_t rows_;
    std::size_t cols_;
    std::unique_ptr<T[]> data_;

    static std::size_t elementCount(std::size_t rows,
                                    std::size_t cols) {
        if (rows != 0 &&
            cols > std::numeric_limits<std::size_t>::max() / rows) {
            throw std::length_error("2D array size overflow");
        }

        return rows * cols;
    }

public:
    Contiguous2DArray(std::size_t rows, std::size_t cols)
        : rows_(rows),
          cols_(cols),
          data_(std::make_unique<T[]>(elementCount(rows, cols))) {}

    T* operator[](std::size_t row) noexcept {
        return data_.get() + row * cols_;
    }

    const T* operator[](std::size_t row) const noexcept {
        return data_.get() + row * cols_;
    }

    std::size_t rows() const noexcept {
        return rows_;
    }

    std::size_t cols() const noexcept {
        return cols_;
    }

    T* data() noexcept {
        return data_.get();
    }

    const T* data() const noexcept {
        return data_.get();
    }
};