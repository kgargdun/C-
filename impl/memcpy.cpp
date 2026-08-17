#include<iostream>

void* my_memcpy(void* dest, const void* src, std::size_t count)
{
    auto* dest_ptr = static_cast<unsigned char*>(dest);
    const auto* src_ptr = static_cast<const unsigned char*>(src);

    constexpr std::size_t alignment = alignof(std::uint64_t);
    constexpr std::size_t word_size = sizeof(std::uint64_t);

    const std::uintptr_t src_addr =
        reinterpret_cast<std::uintptr_t>(src_ptr);

    const std::uintptr_t dest_addr =
        reinterpret_cast<std::uintptr_t>(dest_ptr);

    // Both pointers cannot become aligned simultaneously.
    if (src_addr % alignment != dest_addr % alignment) {
        while (count > 0) {
            *dest_ptr++ = *src_ptr++;
            --count;
        }

        return dest;
    }

    std::size_t leading =
        (alignment - (src_addr % alignment)) % alignment;

    leading = std::min(leading, count);

    for (std::size_t i = 0; i < leading; ++i) {
        *dest_ptr++ = *src_ptr++;
    }

    count -= leading;

    auto* dest_word =
        reinterpret_cast<std::uint64_t*>(dest_ptr);

    const auto* src_word =
        reinterpret_cast<const std::uint64_t*>(src_ptr);

    while (count >= word_size) {
        *dest_word++ = *src_word++;
        count -= word_size;
    }

    dest_ptr = reinterpret_cast<unsigned char*>(dest_word);
    src_ptr = reinterpret_cast<const unsigned char*>(src_word);

    while (count > 0) {
        *dest_ptr++ = *src_ptr++;
        --count;
    }

    return dest;
}