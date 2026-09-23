#include <cstdint> // uint64_t
#include <cassert>  // assert

inline uint64_t expand(uint64_t input, uint32_t scale) {
    assert(scale >= 1);

    uint64_t result = 0;

    for (uint32_t i = 0; i < 64; ++i) {
        uint64_t position = static_cast<uint64_t>(i) * scale;

        if (position >= 64) {
            break;
        }

        uint64_t bit = (input >> i) & 1;
        result |= bit << position;
    }

    return result;
}

inline uint64_t morton3d(uint64_t x, uint64_t y, uint64_t z) {
    uint64_t result = 0;

    result |= expand(x, 3); // I cannot believe it was this simple. I spent so long googling this
    result |= expand(y, 3) << 1;
    result |= expand(z, 3) << 2;

    return result;
}