#include <vector>
#include <cmath>
#include <iostream>
#include <ranges>
#include <algorithm>
#include <filesystem>
#include <execution>
#include <mutex>

#include "lattice_2d.h"
#include "exact_result.h"
#include "utils.h"

constexpr size_t NUM_INV_J_STEPS = 10000;

constexpr size_t NUM_STEPS = 10000;

constexpr double Beta = 1.0;

constexpr double H = 0.0;

constexpr double Critical = std::log(1 + std::numbers::sqrt2) / 2.0;

const std::vector<size_t> LATTICE_SIZES { 4, 8, 12 };

const std::vector SPONTANEOUS_MAGNETIZATION_J { 0.1, 0.2, Critical, 0.7, 0.8 };

/**
 * Calculates the exact magnetization of the 2D Ising model.
 *
 * @param j The coupling constant j.
 * @return The exact magnetization of the 2D Ising model.
 */
double exact_magnetization(const double j) {
    if (j > Critical) {
        return std::pow(1.0 - std::pow(std::sinh(2.0 * j), -4.0), 0.125);
    }
    return 0.0;
}

/**
 * Calculates the exact energy of the 2D Ising model.
 *
 * @param j The coupling constant j.
 * @return The exact energy of the 2D Ising model.
 */
double exact_energy(const double j) {
    const double k = std::comp_ellint_1(2.0 * std::tanh(2.0 * j) / std::cosh(2.0 * j));
    return -j * (std::cosh(2.0 * j) / std::sinh(2.0 * j)) * (1.0 + 2.0 * (2.0 * std::pow(std::tanh(2.0 * j), 2.0) - 1.0) * k / std::numbers::pi);
}

static auto exact_sweep_through_inv_j() {
    static_assert(NUM_INV_J_STEPS > 1);
    return std::views::iota(static_cast<size_t>(0), NUM_INV_J_STEPS) | std::views::transform([=] (const size_t i) {
        return 1.0 / (1.0 + static_cast<double>(i) * (3.0 / static_cast<double>(NUM_INV_J_STEPS - 1)));
    });
}

void calculate_exact_results()
{
    std::cout << "Calculating exact results for Energy and Magnetization" << std::endl;

    std::vector<ExactResult> measurements (NUM_INV_J_STEPS);
    std::ranges::transform(exact_sweep_through_inv_j(), measurements.begin(), [] (const double j) {
        return ExactResult { j, exact_energy(j), exact_magnetization(j) };
    });

    const std::span<const ExactResult> span = measurements;
    write_output_csv(span, "exact_results", "j,energy,magnetization");
}

Lattice2D checkerboard_lattice(const size_t lattice_size, const double j) {
    std::vector<int8_t> spins (lattice_size * lattice_size, 1);
    for (size_t i = 0; i < spins.size(); i += 2) spins.at(i) = -1;
    return Lattice2D { Beta, j, H, spins };
}

std::vector<LatticeObservable> metropolis_fixed_j(const size_t lattice_length, const double j)
{
    std::cout << "\tSimulating for J = " + std::to_string(j) + "\n";

    std::vector<LatticeObservable> measurements (NUM_STEPS);
    std::ranges::transform(checkerboard_lattice(lattice_length, j).sweeps() | std::views::take(NUM_STEPS), measurements.begin(), [=] (const auto current) {
        return current / std::pow(lattice_length, 2.0);
    });

    return measurements;
}

void metropolis_sweep_j(std::vector<double> range, const std::string & prefix) {
    for (const size_t lattice_length : LATTICE_SIZES) {
        std::cout << "Simulating various J for N = " << lattice_length << std::endl;

        std::mutex measurements_mtx;
        std::vector<LatticeObservable> measurements;

        std::for_each(std::execution::par, range.begin(), range.end(), [&] (const double j) mutable {
            const std::vector<LatticeObservable> intermediate = metropolis_fixed_j(lattice_length, j);
            std::lock_guard measurements_lock (measurements_mtx);
            measurements.insert(measurements.end(), intermediate.begin(), intermediate.end());
        });

        const std::span<const LatticeObservable> span = measurements;
        write_output_csv(span, prefix + std::to_string(lattice_length), "j,sweeps,energy,magnetization");
    }
}

static std::vector<double> sweep_through_inv_j() {
    std::vector<double> result (31);
    std::ranges::generate(result, [n = 0.9] mutable{ return 1.0 / (n += 0.1); });
    return result;
}

int main() {
    std::filesystem::create_directory("output");

    calculate_exact_results();
    metropolis_sweep_j(SPONTANEOUS_MAGNETIZATION_J, "6_1_SpontaneousMagnetization_");
    metropolis_sweep_j(sweep_through_inv_j(), "6_2_ScanningJ_");
}