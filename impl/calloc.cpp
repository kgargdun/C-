#include <cstddef>
#include <cstdint>
#include <cstdlib>

void* my_calloc(std::size_t count, std::size_t size) {
    if (size != 0 && count > SIZE_MAX / size) {
        return nullptr;
    }

    std::size_t total = count * size;
    void* ret = std::malloc(total);

    if (!ret) return nullptr;

    auto* bytes = static_cast<unsigned char*>(ret);

    for (std::size_t i = 0; i < total; ++i) {
        bytes[i] = 0;
    }

    return ret;
}