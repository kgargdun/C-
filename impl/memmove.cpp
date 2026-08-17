#include <cstddef>
#include <cstdint>

void* my_memmove(void* dest, const void* src, std::size_t n) {
    auto destAddr = reinterpret_cast<std::uintptr_t>(dest);
    auto srcAddr  = reinterpret_cast<std::uintptr_t>(src);

    auto* d = static_cast<unsigned char*>(dest);
    const auto* s = static_cast<const unsigned char*>(src);

    if (destAddr > srcAddr) {
        // Copy backward
        for (std::size_t i = n; i > 0; --i) {
            d[i - 1] = s[i - 1];
        }
    } else {
        // Copy forward
        for (std::size_t i = 0; i < n; ++i) {
            d[i] = s[i];
        }
    }

    return dest;
}