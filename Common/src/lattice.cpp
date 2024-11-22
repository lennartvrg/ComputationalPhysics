#include <cmath>
#include <algorithm>
#include <random>
#include <ranges>
#include <limits>

#include "lattice.h"

#include <iostream>
#include <memory>

/**
 * The Mersenne Twister 19937 uniform random number generator with random seed.
 */
static thread_local std::mt19937 generator {std::random_device()()};

/**
 * A uniform distribution [0, 1] from which the generate can sample.
 */
static std::uniform_real_distribution uniform_distribution {0.0, 1.0};

double Lattice::action() const {
    return beta * (energy() - h * magnetization());
}

double Lattice::action_diff(const size_t i) const {
    return action_diff(energy_diff(i), magnetization_diff(i));
}

double Lattice::action_diff(const double diff_energy, const double diff_magnetization) const {
    return beta * (diff_energy - h * diff_magnetization);
}

double Lattice::acceptance(const size_t i) const noexcept {
    return std::min(1.0, std::exp(-action_diff(i)));
}

double Lattice::acceptance(const double diff_energy, double const diff_magnetization) const {
    return std::min(1.0, std::exp(-action_diff(diff_energy, diff_magnetization)));
}

std::generator<LatticeObservable> Lattice::sweeps() {
    while (current.sweeps < std::numeric_limits<size_t>::max()) {
        current.sweeps += 1;
        for (const size_t i : std::views::iota(static_cast<size_t>(0), num_sites())) {
            const double diff_energy = energy_diff(i);
            const double diff_magnetization = magnetization_diff(i);

            if (acceptance(diff_energy, diff_magnetization) > uniform_distribution(generator)) {
                current.energy += diff_energy;
                current.magnetization += diff_magnetization;
                flip_spin(i);
            }
        }
        co_yield current;
    }
}

LatticeObservable Lattice::metropolis_hastings(const size_t num_sweeps) {
    return std::ranges::fold_left(sweeps() | std::views::take(num_sweeps), LatticeObservable(), [] (const auto sum, const auto current) {
        return sum + current;
    }) / (num_sites() * num_sweeps);
}
