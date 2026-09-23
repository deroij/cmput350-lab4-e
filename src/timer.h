#ifndef TIMER_H
#define TIMER_H

// replace with yours from the prep...

#include <chrono>

class Timer {
    public:
        using Nanos = std::chrono::nanoseconds;
        using Micros = std::chrono::microseconds;
        using Millis = std::chrono::milliseconds;
        using Seconds = std::chrono::seconds;
        using Minutes = std::chrono::minutes;
        using Hours = std::chrono::hours;

        Timer() {
            restart();
        }

        void restart() {
            start = std::chrono::high_resolution_clock::now();
        }

        template <typename T> uint64_t click() {
            auto end = std::chrono::high_resolution_clock::now();
            auto output = std::chrono::duration_cast<T>(end - start).count();
            start = end;
            return output;
        }

        template <typename T> uint64_t glance() {
            auto end = std::chrono::high_resolution_clock::now();
            return std::chrono::duration_cast<T>(end - start).count();
        }
        
    private:
        std::chrono::high_resolution_clock::time_point start;
};


#endif  // TIMER_H
