#include <cstddef>  // size_t
#include <cstdint> // uint64_t
#include <random> // mt19937_64
#include <array>
#include <iostream>

#include "timer.h"

constexpr size_t ARRAY_SIZE = 4000;

int main() {
    uint64_t (*contiguousarray1)[ARRAY_SIZE] = new uint64_t[ARRAY_SIZE][ARRAY_SIZE]; //This was a pain in the ass to find
    uint64_t (*contiguousarray2)[ARRAY_SIZE] = new uint64_t[ARRAY_SIZE][ARRAY_SIZE];
    Timer timer;
    std::mt19937_64 rng(0);

    for (size_t i = 0; i < ARRAY_SIZE; ++i){
        for (size_t j = 0; j < ARRAY_SIZE; ++j){
            contiguousarray1[i][j] = rng();
        }
    }

    rng.seed(0);
    for (size_t i = 0; i < ARRAY_SIZE; ++i){
        for (size_t j = 0; j < ARRAY_SIZE; ++j){
            contiguousarray2[j][i] = rng();
        }
    }

    uint64_t sum1 = 0;
    timer.restart();
    for (size_t i = 0; i < ARRAY_SIZE; ++i){
        for (size_t j = 0; j < ARRAY_SIZE; ++j){
            sum1 += contiguousarray1[i][j];
        }
    }
    uint64_t time1 = timer.click<Timer::Micros>();

    uint64_t sum2 = 0;
    timer.restart();
    for (size_t i = 0; i < ARRAY_SIZE; ++i){
        for (size_t j = 0; j < ARRAY_SIZE; ++j){    
            sum2 += contiguousarray2[i][j];
        }
    }
    uint64_t time2 = timer.click<Timer::Micros>();

    std::cout << time1 << " " << sum1 << "\n";
    std::cout << time2 << " " << sum2 << "\n";
    return 0;
}
