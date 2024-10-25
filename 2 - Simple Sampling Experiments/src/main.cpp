#include <iostream>
#include <random>
#include <fstream>
#include <format>
#include <filesystem>

static std::mt19937 generator; 
static std::uniform_int_distribution<int> uniform_distribution (0, 1);

template<std::size_t N>
const double coinflip() {
    double result {0};
    for (size_t j = 1; j <= N; ++j) {
        result += uniform_distribution(generator) / pow(2, j);
    }
    return result;
}

template<std::size_t S>
const void sequence_of_coinflips() {
    std::ofstream output;
    output.open(std::format("output/sequence{}.csv", S));
    
    std::cout << "Generting sequence of " << powl(10, S) << " real numbers" << std::endl;
    for (size_t s = 0; s < powl(10, S); ++s) {
        output << coinflip<32>() << std::endl;
    }
    output.close();
}

template<std::size_t N>
const double biased_coinflip(const double lambda) {
    double result {0};
    for (size_t j = 1; j <= N; ++j) {
        double p = 1.0 / (1.0 + exp(-lambda / pow(2, j+1)));
        std::discrete_distribution<int> biased_distribution ({1.0 - p, p});

        result += biased_distribution(generator) / pow(2, j);
    }
    return result;
}

const void sequence_of_biased_coinflips(const double lambda) {
    std::ofstream output;
    output.open(std::format("output/sequence_biased{:.1f}.csv", lambda));
    
    std::cout << "Generting biased sequence of " << powl(10, 6) << " real numbers" << std::endl;
    for (size_t s = 0; s < powl(10, 6); ++s) {
        output << biased_coinflip<32>(lambda) << std::endl;
    }
    output.close();
}

int main() {
    std::filesystem::create_directory("output");

    sequence_of_coinflips<2>();
    sequence_of_coinflips<4>();
    sequence_of_coinflips<6>();

    sequence_of_biased_coinflips(0.0);
    sequence_of_biased_coinflips(0.5);
    sequence_of_biased_coinflips(1.0);
    sequence_of_biased_coinflips(2.0);
}