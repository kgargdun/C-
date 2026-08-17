#include <cstddef>

int my_strcmp(const char* first, const char* second) {
    while (*first != '\0' && *first == *second) {
        ++first;
        ++second;
    }

    return static_cast<unsigned char>(*first) -
           static_cast<unsigned char>(*second);
}

char* my_strncpy(char* destination,
                 const char* source,
                 std::size_t count) {
    std::size_t i = 0;

    // Copy until source ends or count is reached.
    while (i < count && source[i] != '\0') {
        destination[i] = source[i];
        ++i;
    }

    // Pad remaining space with null characters.
    while (i < count) {
        destination[i] = '\0';
        ++i;
    }

    return destination;
}