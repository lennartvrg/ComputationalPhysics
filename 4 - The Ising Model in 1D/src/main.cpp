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
#include <lattice_measurement.h>
#include <lattice_1d.h>
#include <metropolis_result.h>
#include <utils.h>

constexpr size_t NUM_H_STEPS = 100;

constexpr size_t NUM_EXPERIMENTS = 100;

constexpr size_t NUM_SAMPLES = 10000;

constexpr size_t LATTICE_SIZE = 20;

constexpr double J = 0.75;

/**
 * The Mersenne Twister 19937 uniform random number generator with random seed.
 */
static thread_local std::mt19937 generator {std::random_device()()};

/**
 * A uniform distribution [0, 1] from which the generate can sample.
 */
static std::uniform_real_distribution uniform_distribution {0.0, 1.0};

static auto stepped_magnetic_field() {
	return std::views::iota(static_cast<size_t>(0), NUM_H_STEPS) | std::views::transform([=] (const size_t i) {
		return static_cast<double>(2 * i) / static_cast<double>(NUM_H_STEPS - 1) - 1.0;
	});
}

/**
 * 
 * @param lattice_size The size of the lattice to be measured.
 * @return The time it took to calculate the action and action_diff in nanoseconds.
 */
LatticeMeasurement measure_lattice(const size_t lattice_size)
{
	static std::atomic_int counter { 0 };
	const Lattice1D lattice { lattice_size, 1, 1 };

	const Experiment<int64_t> action = measure_execution([&] -> void {
		static_cast<void>(lattice.action());
	}, 100);

	const Experiment<int64_t> diffAction = measure_execution([&] -> void {
		static_cast<void>(lattice.action_diff(0));
	}, 100);

	std::cout << "\rMeasure lattice scaling: " << std::to_string(++counter) << "/50" << std::flush;
	return LatticeMeasurement { lattice_size, action, diffAction };
}

void measure_lattice_scaling()
{
	std::vector<size_t> sizes (10);
	std::ranges::generate(sizes, [n = 0] mutable -> int { return n += 100000; });

	std::vector<LatticeMeasurement> measurements (sizes.size());
	std::ranges::transform(sizes, measurements.begin(), measure_lattice);

	write_output_csv(static_cast<std::span<LatticeMeasurement>>(measurements), "lattice_scaling", "Lattice,Action,DeltaAction,DiffAction,DeltaDiffAction");
	std::cout << std::endl;
}

void metropolis_sweep(const std::shared_ptr<Lattice> & lattice)
{
	for (const size_t i : std::views::iota(static_cast<size_t>(0), lattice->num_sites())) {
		if (lattice->acceptance(i) > uniform_distribution(generator)) {
			lattice->fetch_flip(i);
		}
	}
}

double metropolis_hastings_single_experiment(const size_t num_samples, const double h)
{
	std::ranges::iota_view<size_t, size_t> ranges { 0, num_samples };
	const std::shared_ptr<Lattice> lattice = std::make_shared<Lattice1D>(LATTICE_SIZE, J, h);

	return std::accumulate(ranges.begin(), ranges.end(), 0.0, [&] (const double sum, [[maybe_unused]] const size_t i) {
		metropolis_sweep(lattice);
		return sum + lattice->magnetization();
	}) / static_cast<double>(lattice->num_sites() * num_samples);
}

MetropolisResult metropolis_hastings_multiple_experiments(const size_t num_experiments, const size_t num_samples, const double h)
{
	std::vector<double> measurements (num_experiments);
	std::generate(std::execution::par, measurements.begin(), measurements.end(), [&] {
		return metropolis_hastings_single_experiment(num_samples, h);
	});
	return MetropolisResult { h, Experiment<double>(measurements) };
}

void sweep_external_magnetic_field() {
	static std::atomic_int counter { 0 };
	std::vector<MetropolisResult> measurements;

	for (const double h : stepped_magnetic_field()) {
		std::cout << "\rMetropolis-Hastings: h=" << std::to_string(++counter) << "/" << std::to_string(NUM_H_STEPS) << std::flush;
		measurements.emplace_back(metropolis_hastings_multiple_experiments(NUM_EXPERIMENTS, NUM_SAMPLES, h));
	}

	write_output_csv(static_cast<std::span<MetropolisResult>>(measurements), "metropolis", "h,magnetization,delta_magnetization");
	std::cout << std::endl;
}

int main()
{
    std::filesystem::create_directory("output");

	measure_lattice_scaling();
	sweep_external_magnetic_field();

	return 0;
}