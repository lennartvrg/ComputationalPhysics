#include <algorithm>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <span>
#include <string>
#include <execution>
#include <random>

#include <measurement.h>
#include <lattice_measurement.h>
#include <lattice_1d.h>
#include <utils.h>

/**
 * The Mersenne Twister 19937 uniform random number generator with random seed.
 */
static thread_local std::mt19937 generator {std::random_device()()};

/**
 * A uniform distribution [0, 1] from which the generate can sample.
 */
static std::uniform_real_distribution uniform_distribution {0.0, 1.0};

/**
 * 
 * @param lattice_size The size of the lattice to be measured.
 * @return The time it took to calculate the action and action_diff in nanoseconds.
 */
LatticeMeasurement measure_lattice(const size_t lattice_size)
{
	static std::atomic_int counter { 0 };
	const Lattice1D lattice { lattice_size, 1, 1 };

	const Measurement<int64_t> action = measure_execution([&] -> void {
		static_cast<void>(lattice.action());
	}, 100);

	const Measurement<int64_t> diffAction = measure_execution([&] -> void {
		static_cast<void>(lattice.action_diff(0));
	}, 100);

	std::cout << "\rMeasure lattice scaling: " + std::to_string(++counter) + "/50" << std::flush;
	return LatticeMeasurement { lattice_size, action, diffAction };
}

void measure_lattice_scaling()
{
	std::vector<size_t> sizes (50);
	std::ranges::generate(sizes, [n = 0] mutable -> int { return n += 200000; });

	std::vector<LatticeMeasurement> measurements (sizes.size());
	std::ranges::transform(sizes, measurements.begin(), measure_lattice);

	write_output_csv(measurements, "lattice_scaling", "Lattice,Action,DeltaAction,DiffAction,DeltaDiffAction\n");
	std::cout << std::endl;
}

void metropolis_sweep(const std::shared_ptr<Lattice> & lattice)
{
	for (size_t i = 0; i < lattice->num_sites(); ++i) {
		const double diff = lattice->action_diff(i);
		const double acceptance = std::min(1.0, std::exp(-diff));

		if (acceptance > uniform_distribution(generator)) {
			lattice->fetch_flip(i);
		}
	}
}

double metropolis_hastings(const size_t num_samples, const double h)
{
	std::vector<double> measurements (num_samples);
	const std::shared_ptr<Lattice> lattice = std::make_shared<Lattice1D>(20, 0.75, h);

	for (size_t i = 0; i < num_samples; ++i) {
		metropolis_sweep(lattice);
		measurements.push_back(lattice->magnetization() / static_cast<double>(lattice->num_sites()));
	}

	return std::accumulate(measurements.begin(), measurements.end(), 0.0) / static_cast<double>(measurements.size());
}

void sweep_external_magnetic_fields() {
	std::ofstream output;
	output.open("output/metropolis.csv");

	output << "h,value" << "\n";
	for (size_t i = 0; i <= 200; ++i) {
		const double h = static_cast<double>(i) / 100 - 1.0;
		output << h << "," << metropolis_hastings(100000, h) << "\n";
	}

	output.close();
}

int main()
{
    std::filesystem::create_directory("output");

	//measure_lattice_scaling();
	sweep_external_magnetic_fields();

	return 0;
}