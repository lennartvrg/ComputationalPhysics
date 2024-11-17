#ifndef EXACT_RESULT_H
#define EXACT_RESULT_H

#include <ostream>
#include <sstream>

struct ExactResult {
    ExactResult() = default;
    explicit ExactResult(const double j, const double energy, const double magnetization) : j(j), energy(energy), magnetization(magnetization) {};

    friend std::ostream & operator<<(std::ostream & os, const ExactResult & result) {
        std::stringstream output;
        output << result.j << "," << result.energy << "," << result.magnetization;
        return os << output.str();
    }

private:
    double j, energy, magnetization;
};

#endif //EXACT_RESULT_H
