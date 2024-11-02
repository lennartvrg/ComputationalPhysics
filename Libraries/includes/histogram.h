#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <vector>
#include <atomic>
#include <cstddef>
#include <iostream>
#include <ranges>

namespace histogram
{
    class Histogram
    {
    public:
        explicit Histogram(size_t bins);

        void add(double value);
        friend std::ostream & operator<<(std::ostream & os, const Histogram & histogram);

    private:
        std::vector<std::atomic<int32_t>> data;
    };

    std::ostream & operator<<(std::ostream & os, const Histogram & histogram);
}

#endif //HISTOGRAM_H
