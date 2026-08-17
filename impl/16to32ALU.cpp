#include <cstdint>

uint32_t multiply16(uint16_t a, uint16_t b) {
    // A 16×16 multiplication produces a 32-bit result.
    return static_cast<uint32_t>(a) *
           static_cast<uint32_t>(b);
}

uint64_t multiply32(uint32_t a, uint32_t b) {
    uint16_t aLow  = static_cast<uint16_t>(a);
    uint16_t aHigh = static_cast<uint16_t>(a >> 16);

    uint16_t bLow  = static_cast<uint16_t>(b);
    uint16_t bHigh = static_cast<uint16_t>(b >> 16);

    uint32_t lowLow   = multiply16(aLow,  bLow);
    uint32_t highLow  = multiply16(aHigh, bLow);
    uint32_t lowHigh  = multiply16(aLow,  bHigh);
    uint32_t highHigh = multiply16(aHigh, bHigh);

    return static_cast<uint64_t>(lowLow)
         + (static_cast<uint64_t>(highLow)  << 16)
         + (static_cast<uint64_t>(lowHigh)  << 16)
         + (static_cast<uint64_t>(highHigh) << 32);
}