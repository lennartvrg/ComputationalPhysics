#include "histogram.h"

/**
 * Instantiates a new histogram with the given number of bins.
 * 
 * @param bins The number of bins in the histogram
 */
histogram::Histogram::Histogram(const size_t bins) : data(bins)
{
    std::ranges::fill(data, 0);
}

/**
 * Adds a random number to the histogram. That number must be on the interval [0, 1).
 *
 * @param value The value to be added to the histogram. Must be in the interval [0, 1).
 */
void histogram::Histogram::add(const double value)
{
    data.at(static_cast<std::size_t>(static_cast<double>(data.size()) * value)).fetch_add(1, std::memory_order_relaxed);
}

/**
 * Writes the histogram data into the output stream in CSV format. Starts by adding the
 * headers 'Bin,Count' and then adds the bins and counts themselves line-by-line.
 *
 * @param os The output stream to which the histogram should be written.
 * @param histogram The histogram object which gets written to the output.
 * @return The output stream after the histogram was written to it.
 */
std::ostream & histogram::operator<<(std::ostream & os, const Histogram & histogram)
{
    os << "Bin,Count\n";
    std::for_each(histogram.data.begin(), histogram.data.end(), [idx = 0, & histogram, & os] (const std::atomic<int32_t> & value) mutable {
        os << static_cast<double>(idx++) / static_cast<double>(histogram.data.size()) << "," << value << "\n";
    });
    return os;
}