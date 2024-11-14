#include <algorithm>
#include <atomic>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <span>
#include <string>
#include <random>
#include <ranges>
#include <execution>

#include <experiment.h>
#include <lattice_result.h>
#include <lattice_1d.h>
#include <metropolis_result.h>
#include <utils.h>

/**
 * The number of steps the range [-1,+1] of the external magnetic field is divided into.
 */
constexpr size_t NUM_H_STEPS = 100;

/**
 * The number of experiments done
 */
constexpr size_t NUM_EXPERIMENTS = 100;

/**
 * The number of sweeps for each experiment.
 */
constexpr size_t NUM_SWEEPS = 10000;

/**
 * The lattice size.
 */
constexpr size_t LATTICE_SIZE = 20;

/**
 * The coupling constant J
 */
constexpr double J = 0.75;

/**
 * The Mersenne Twister 19937 uniform random number generator with random seed.
 */
static thread_local std::mt19937 generator {std::random_device()()};

/**
 * A uniform distribution [0, 1] from which the generate can sample.
 */
static std::uniform_real_distribution uniform_distribution {0.0, 1.0};

/**
 * Divides the range [-1,+1] of the external magnetic field into NUM_H_STEPS steps for iterating over them.
 *
 * @return The strenght of the external magnetic field.
 */
static auto stepped_magnetic_field() {
	return std::views::iota(static_cast<size_t>(0), NUM_H_STEPS) | std::views::transform([=] (const size_t i) {
		return static_cast<double>(2 * i) / static_cast<double>(NUM_H_STEPS - 1) - 1.0;
	});
}

/**
 * Calculates how long calculation the action and the delta action takes for a given lattice size.
 *
 * @param lattice_size The size of the lattice to be measured.
 * @return The time it took to calculate the action and action_diff in nanoseconds.
 */
LatticeResult measure_lattice(const size_t lattice_size)
{
	static std::atomic_int counter { 0 };
	const Lattice1D lattice { lattice_size, J, 0 };

	const Experiment<int64_t> action = measure_execution([&] -> void {
		static_cast<void>(lattice.action());
	}, 20);

	const Experiment<int64_t> diffAction = measure_execution([&] -> void {
		static_cast<void>(lattice.action_diff(0));
	}, 20);

	std::cout << "\rMeasure lattice scaling: " << std::to_string(++counter) << "/100" << std::flush;
	return LatticeResult { lattice_size, action, diffAction };
}

/**
 * Measures how long it takes to calculate the action for increasing lattice sizes.
 */
void measure_lattice_scaling()
{
	std::vector<size_t> sizes (100);
	std::ranges::generate(sizes, [n = 0] mutable -> int { return n += 50000; });

	std::vector<LatticeResult> measurements (sizes.size());
	std::ranges::transform(sizes, measurements.begin(), measure_lattice);
	std::cout << std::endl;

	const std::span<const LatticeResult> span = measurements;
	write_output_csv(span, "lattice_scaling", "Lattice,Action,DeltaAction,DiffAction,DeltaDiffAction");
}

/**
 * Performs a single lattice sweep and calculates the acceptance ratio for every lattice site and flips
 * the spin of the site if the acceptance ration is greater than a random number [0, 1].
 */
double metropolis_sweep(const std::shared_ptr<Lattice> & lattice)
{
	double delta = 0.0;
	for (const size_t i : std::views::iota(static_cast<size_t>(0), lattice->num_sites())) {
		if (lattice->acceptance(i) > uniform_distribution(generator)) {
			delta += lattice->flip_fetch_magnetization_diff(i);
		}
	}
	return delta;
}

/**
 * Performs the metropolis hastings simulation with the given number of sweeps for a given
 * external magnetic field h. Returns the mean magnetization per spin.
 *
 * @param num_sweeps The number of sweeps made in total.
 * @param h The strength of the external magnetic field.
 * @return The mean magnetization per spin.
 */
double metropolis_hastings_single_experiment(const size_t num_sweeps, const double h)
{
	std::ranges::iota_view<size_t, size_t> ranges { 0, num_sweeps };
	const std::shared_ptr<Lattice> lattice = std::make_shared<Lattice1D>(LATTICE_SIZE, J, h);

	return std::accumulate(ranges.begin(), ranges.end(), 0.0, [&, current = lattice->magnetization()] (const double sum, [[maybe_unused]] const size_t i) mutable {
		return sum + (current += metropolis_sweep(lattice));
	}) / static_cast<double>(lattice->num_sites() * num_sweeps);
}

/**
 * Performs the metropolis hastings simulation multiple times for a given external magnetic field.
 * Returns the mean magnetization and uncertainty per spin across all runs.
 *
 * @param num_experiments The number of experiments to perform.
 * @param num_samples The number of sweeps per experiment.
 * @param h The strength of the external magnetic field.
 * @return The mean magnetization and uncertainty per spin.
 */
MetropolisResult metropolis_hastings_multiple_experiments(const size_t num_experiments, const size_t num_samples, const double h)
{
	std::vector<double> measurements (num_experiments);
	std::generate(std::execution::par, measurements.begin(), measurements.end(), [&] {
		return metropolis_hastings_single_experiment(num_samples, h);
	});
	return MetropolisResult { h, Experiment<double>(measurements) };
}

/**
 * Sweeps through the external magnetic field [-1,+1] and calculates the mean magnetization and uncertainty per spin
 * per value of h and writes the results to a CSV file.
 */
void sweep_external_magnetic_field() {
	static std::atomic_int counter { 0 };
	std::vector<MetropolisResult> measurements;

	for (const double h : stepped_magnetic_field()) {
		std::cout << "\rMetropolis-Hastings: " << std::to_string(++counter) << "/" << std::to_string(NUM_H_STEPS) << std::flush;
		measurements.emplace_back(metropolis_hastings_multiple_experiments(NUM_EXPERIMENTS, NUM_SWEEPS, h));
	}
	std::cout << std::endl;

	const std::span<const MetropolisResult> span = measurements;
	write_output_csv(span, "metropolis", "h,magnetization,delta_magnetization");
}

/**
 * Runs code for problem set 4.
 *
 * @return System status code.
 */
int main()
{
    std::filesystem::create_directory("output");

	measure_lattice_scaling();
	sweep_external_magnetic_field();

	return 0;
}