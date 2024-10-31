#include <cmath>
#include <algorithm>
#include <array>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <random>
#include <ranges>
#include <span>
#include <string>
#include <execution>

/**
 * The Mersenne Twister 19937 uniform random number generator with random seed.
 */
static thread_local std::mt19937 generator {std::random_device()()};

/**
 * A uniform distribution [0, 1] from which the generate can sample.
 */
static std::uniform_int_distribution<int> uniform_distribution {0, 1};

/**
 * The iterator on the integer interval [0, 32) used for the number of coinflips
 */
static std::ranges::iota_view flips {0, 32};

/**
 * Copies the given span of double precision numbers into the output
 * CSV file with the given name.
 *
 * @param numbers The span of double precision numbers.
 * @param name The name of the output file.
 */
void write_output(const std::span<double> numbers, const std::string & name) {
    std::ofstream output;
    output.open("output/" + name + ".csv");

    std::ranges::copy(numbers.begin(), numbers.end(), std::ostream_iterator<double>(output, "\n"));
    output.close();
}

/**
 * Generates a uniform random real number on the interval [0, 1) by flipping a coin 32 times.
 * The results of this flip are accumulated according to $real = \sum^{32}_{j=1}{\frac{f_j}{2^{j}}}$.
 *
 * @return A uniform random real number on the interval [0, 1)
 */
double uniform_real()
{
    return std::accumulate(flips.begin(), flips.end(), 0.0, [&] (const double acc, const int j) {
        return acc + uniform_distribution(generator) / pow(2, j + 1);
    });
}

/**
 * Generates a sequence of uniform reals in parallel and writes the result to a CSV file.
 *
 * @tparam S The size of the sequence of uniform reals.
 */
template <std::size_t S>
void sequence_of_uniform_reals()
{
    std::cout << "\t S is " << S << std::endl;

    std::array<double, S> numbers {};
    std::generate(std::execution::par_unseq, numbers.begin(), numbers.end(), uniform_real);

    write_output(numbers, "sequence" + std::to_string(S));
}

/**
 * Generates a biased random real number on the interval [0, 1) by flipping a coin 32 times.
 * The results of this flip are accumulated according to $real = \sum^{32}_{j=1}{\frac{f_j}{2^{j}}}$.
 *
 * @param distributions The array of biased distributions. Has the same length as the number of coinflips used.
 * @return A biased random real number on the interval [0, 1)
 */
double biased_real(std::array<std::discrete_distribution<>, 32> & distributions)
{
    return std::accumulate(flips.begin(), flips.end(), 0.0, [&] (const double acc, const int j) {
        return acc + distributions.at(j)(generator) / pow(2, j + 1);
    });
}

/**
 * Generates a sequence of biased reals in parallel and writes the result to a CSV file. The biased distributions are
 * calculated once for evey possible value of j and then passed to the function calculating the biased real number.
 *
 * @tparam S The size of the sequence of biased reals.
 * @param lambda The lambda parameter of the bias.
 */
template <std::size_t S>
void sequence_of_biased_reals(const double lambda)
{
    std::cout << "\t Lambda is " << lambda << std::endl;

    std::array<std::discrete_distribution<>, 32> distributions {};
    std::transform(flips.begin(), flips.end(), distributions.begin(), [&] (const int j) {
        double p = 1.0 / (1.0 + exp(-lambda / pow(2, j + 1)));
        return std::discrete_distribution({p, 1.0 - p});
    });

    std::array<double, S> numbers {};
    std::generate(std::execution::par_unseq, numbers.begin(), numbers.end(), [&] {
        return biased_real(distributions);
    });

    write_output(numbers, "sequence_biased" + std::to_string((int) std::round(lambda * 10)));
}

/**
 * Runs problems 2.1 and 2.2 from problem set 2.
 *
 * @return Result codes which indicates to the OS if the execution was successful.
 */
int main()
{
    std::filesystem::create_directory("output");

    std::cout << "2.1: Generating sequences of real numbers" << std::endl;
    sequence_of_uniform_reals<100>();
    sequence_of_uniform_reals<10000>();
    sequence_of_uniform_reals<1000000>();

    std::cout << "2.2: Generating biased sequences of real numbers" << std::endl;
    sequence_of_biased_reals<1000000>(0.0);
    sequence_of_biased_reals<1000000>(0.5);
    sequence_of_biased_reals<1000000>(1.0);
    sequence_of_biased_reals<1000000>(2.0);
}