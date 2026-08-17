#include <cstdint>
#include <cstddef>
#include <iostream>

bool write_32u(uint8_t* buffer,
               size_t size,
               size_t& offset,
               uint32_t val) {
    if (!buffer || offset > size || size - offset < sizeof(uint32_t))
        return false;

    for (size_t i = 0; i < sizeof(uint32_t); ++i) {
        size_t shift = 8 * (sizeof(uint32_t) - 1 - i);

        buffer[offset + i] =
            static_cast<uint8_t>(val >> shift);
    }

    offset += sizeof(uint32_t);
    return true;
}

bool read_32u(const uint8_t* buffer,
              size_t size,
              size_t& offset,
              uint32_t& val) {
    if (!buffer || offset > size || size - offset < sizeof(uint32_t))
        return false;

    uint32_t result = 0;

    for (size_t i = 0; i < sizeof(uint32_t); ++i) {
        result = (result << 8) |
                 static_cast<uint32_t>(buffer[offset + i]);
    }

    offset += sizeof(uint32_t);
    val = result;
    return true;
}

int main() {
    uint8_t buffer[20]{};

    size_t writeOffset = 5;

    if (!write_32u(buffer, sizeof(buffer), writeOffset, 789)) {
        std::cout << "Serialization failed\n";
        return 1;
    }

    size_t readOffset = 5;
    uint32_t value = 0;

    if (!read_32u(buffer, sizeof(buffer), readOffset, value)) {
        std::cout << "Deserialization failed\n";
        return 1;
    }

    std::cout << value << '\n';  // 789

    return 0;
}