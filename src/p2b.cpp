#include <cstddef>  // size_t
#include <cstdint> // uint64_t
#include <random> // mt19937_64
#include <iostream>

#include "timer.h"
#include "p2a.h"

constexpr size_t ARRAY_SIZE = 256;
constexpr size_t KERNEL_SIZE = 4;
constexpr size_t OUTPUT_SIZE = 64; // floor((x;y;z-4) / 4) + 1 = 64.

int main() {
    uint64_t (*contiguousarray)[ARRAY_SIZE][ARRAY_SIZE] = new uint64_t[ARRAY_SIZE][ARRAY_SIZE][ARRAY_SIZE];
    uint64_t (*contiguousarray_out)[OUTPUT_SIZE][OUTPUT_SIZE] = new uint64_t[OUTPUT_SIZE][OUTPUT_SIZE][OUTPUT_SIZE];
    uint64_t *mortonarray = new uint64_t[ARRAY_SIZE * ARRAY_SIZE * ARRAY_SIZE];
    uint64_t *mortonarray_out = new uint64_t[OUTPUT_SIZE * OUTPUT_SIZE * OUTPUT_SIZE];
    Timer timer;
    std::mt19937_64 rng(0);

    for (uint64_t z = 0; z < ARRAY_SIZE; ++z) {
        for (uint64_t y = 0; y < ARRAY_SIZE; ++y) {
            for (uint64_t x = 0; x < ARRAY_SIZE; ++x) {
                contiguousarray[z][y][x] = rng(); // Array order
                mortonarray[morton3d(x, y, z)] = contiguousarray[z][y][x]; // Morton conversion
            }
        }
    }

    // Kernel Creation
    uint64_t Ka[KERNEL_SIZE][KERNEL_SIZE][KERNEL_SIZE]; // Array Kernel
    uint64_t Kb[KERNEL_SIZE * KERNEL_SIZE * KERNEL_SIZE]; // Morton Kernel

    for (uint64_t z = 0; z < KERNEL_SIZE; ++z) {
        for (uint64_t y = 0; y < KERNEL_SIZE; ++y) {
            for (uint64_t x = 0; x < KERNEL_SIZE; ++x) {

                uint64_t value = x + y + z;

                Ka[z][y][x] = value;
                Kb[morton3d(x, y, z)] = value;
            }
        }
    }

    // Convolution tests
    timer.restart();
    for (uint64_t z = 0; z < OUTPUT_SIZE; ++z) {
        for (uint64_t y = 0; y < OUTPUT_SIZE; ++y) {
            for (uint64_t x = 0; x < OUTPUT_SIZE; ++x) {

                uint64_t sum = 0;
                for (uint64_t kz = 0; kz < KERNEL_SIZE; ++kz) {
                    for (uint64_t ky = 0; ky < KERNEL_SIZE; ++ky) {
                        for (uint64_t kx = 0; kx < KERNEL_SIZE; ++kx) {
                            sum += contiguousarray[z * KERNEL_SIZE + kz][y * KERNEL_SIZE + ky][x * KERNEL_SIZE + kx] * Ka[kz][ky][kx];
                        }
                    }
                }
                contiguousarray_out[z][y][x] = sum;
            }
        }
    }
    uint64_t time1 = timer.click<Timer::Micros>();

    timer.restart();
    for (uint64_t z = 0; z < OUTPUT_SIZE; ++z) {
        for (uint64_t y = 0; y < OUTPUT_SIZE; ++y) {
            for (uint64_t x = 0; x < OUTPUT_SIZE; ++x) {
                
                uint64_t sum = 0;
                for (uint64_t kz = 0; kz < KERNEL_SIZE; ++kz) {
                    for (uint64_t ky = 0; ky < KERNEL_SIZE; ++ky) {
                        for (uint64_t kx = 0; kx < KERNEL_SIZE; ++kx) {
                            sum += mortonarray[morton3d(x * KERNEL_SIZE + kx, y * KERNEL_SIZE + ky, z * KERNEL_SIZE + kz)] * Kb[morton3d(kx, ky, kz)];
                        }
                    }
                }
                mortonarray_out[morton3d(x, y, z)] = sum;
            }
        }
    }
    uint64_t time2 = timer.click<Timer::Micros>();

    // Test equality
    for (uint64_t z = 0; z < OUTPUT_SIZE; ++z) {
        for (uint64_t y = 0; y < OUTPUT_SIZE; ++y) {
            for (uint64_t x = 0; x < OUTPUT_SIZE; ++x) {
                assert(contiguousarray_out[z][y][x] == mortonarray_out[morton3d(x, y, z)]);
            }
        }
    }

    std::cout << time1 << "\n" << time2 << "\n";

    delete[] contiguousarray;
    delete[] contiguousarray_out;
    delete[] mortonarray;
    delete[] mortonarray_out;

    return 0;
}
