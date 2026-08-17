#include <cstdint>

void set_bit(uint32_t& reg, int bit) {
    reg |= (1u << bit);
}

void clear_bit(uint32_t& reg, int bit) {
    reg &= ~(1u << bit);
}

void toggle_bit(uint32_t& reg, int bit) {
    reg ^= (1u << bit);
}

bool test_bit(uint32_t reg, int bit) {
    return (reg & (1u << bit)) != 0;
}

uint32_t extract_bits(uint32_t reg, int start, int width) {
    uint32_t mask = (1ULL << width) - 1;
    return (reg >> start) & mask;
}

void replace_bits(uint32_t& reg, int start, int width, uint32_t value) {
    uint32_t mask = (1ULL << width) - 1;

    reg &= ~(mask << start);                 // Clear the field
    reg |= (value & mask) << start;          // Insert new value
}