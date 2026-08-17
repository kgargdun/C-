#include <cstdint>
#include <iostream>
#include <iomanip>

bool is_little_endian() {
    const uint32_t value = 1;

    const unsigned char* firstByte =
        reinterpret_cast<const unsigned char*>(&value);

    return *firstByte == 1;
}

constexpr uint32_t byte_swap(uint32_t value) noexcept {
    uint32_t result = 0;

    for (int i = 0; i < 4; ++i) {
        uint32_t byte = (value >> (8 * i)) & 0xffu;
        result |= byte << (8 * (3 - i));
    }

    return result;
}

uint32_t host_to_big_endian(uint32_t value) {
    if (is_little_endian()) {
        return byte_swap(value);
    }

    return value;
}

int main() {
    uint32_t value = 0x12345678u;

    std::cout << std::boolalpha;
    std::cout << "Little-endian: " << is_little_endian() << '\n';

    std::cout << std::hex << std::showbase;
    std::cout << "Original:      " << value << '\n';
    std::cout << "Byte-swapped:  " << byte_swap(value) << '\n';
    std::cout << "Big-endian:    " << host_to_big_endian(value) << '\n';
}