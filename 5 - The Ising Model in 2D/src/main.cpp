#include <numbers>
#include <cmath>
#include <ranges>
#include <vector>
#include <filesystem>
#include <execution>

#include <utils.h>
#include <exact_result.h>
#include <lattice.h>
#include "lattice_2d.h"
#include "lattice_observable.h"

constexpr size_t NUM_INV_J_STEPS = 10000;

constexpr size_t NUM_STEPS = 100000;

constexpr double Beta = 1.0;

constexpr double J = 0.5;

constexpr double H = 0.0;

constexpr double Critical = std::log(1 + std::numbers::sqrt2) / 2.0;

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

static auto sweep_through_inv_j() {
	static_assert(NUM_INV_J_STEPS > 1);
	return std::views::iota(static_cast<size_t>(0), NUM_INV_J_STEPS) | std::views::transform([=] (const size_t i) {
		return 1.0 / (0.1 + static_cast<double>(i) * (4.9 / static_cast<double>(NUM_INV_J_STEPS - 1)));
	});
}

void calculate_exact_results()
{
	std::cout << "Calculating exact results for Energy and Magnetization" << std::endl;

	std::vector<ExactResult> measurements (NUM_INV_J_STEPS);
	std::ranges::transform(sweep_through_inv_j(), measurements.begin(), [] (const double j) {
		return ExactResult { j, exact_energy(j), exact_magnetization(j) };
	});

	const std::span<const ExactResult> span = measurements;
	write_output_csv(span, "exact_results", "j,energy,magnetization");
}

void monte_carlo_history(const size_t lattice_length)
{
	std::cout << "Metropolis-Hastings for N = " << lattice_length << std::endl;

	std::vector<LatticeObservable> measurements (NUM_STEPS);
	std::ranges::transform(Lattice2D(lattice_length, Beta, J, H).sweeps() | std::views::take(NUM_STEPS), measurements.begin(), [=] (const auto current) {
		return current / std::pow(lattice_length, 2.0);
	});

	const std::span<const LatticeObservable> span = measurements;
	write_output_csv(span, "history_" + std::to_string(lattice_length), "j,sweeps,energy,magnetization");
}

int main()
{
	std::filesystem::create_directory("output");

	calculate_exact_results();
	monte_carlo_history(4);
	monte_carlo_history(8);
	monte_carlo_history(12);
}
