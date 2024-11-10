#include <algorithm>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <numeric>
#include <random>
#include <span>
#include <string>
#include <execution>

#include "measurement.h"
#include <lattice_measurement.h>
#include "lattice_1d.h"

void write_measurement_output(const std::span<LatticeMeasurement> measurements) {
    std::ofstream output;
    output.open("output/lattice_scaling.csv");

	output << "Lattice,Action,DeltaAction,DiffAction,DeltaDiffAction\n";
	std::ranges::copy(measurements, std::ostream_iterator<LatticeMeasurement>(output, "\n"));
    output.close();
}

Measurement<int64_t> measure_execution(const size_t num_runs, const auto & lambda) {
	std::vector<int64_t> measurements (num_runs, 0);
	std::generate(std::execution::par, measurements.begin(), measurements.end(), [&] {
		const auto begin = std::chrono::steady_clock::now();
		static_cast<void>(lambda());

		const std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		return std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
	});
	return Measurement(static_cast<std::span<int64_t>>(measurements));
}

LatticeMeasurement measure_lattice(const size_t lattice_size)
{
	const Lattice1D lattice { lattice_size, 1, 1 };

	const Measurement<int64_t> action = measure_execution(100, [&] -> double {
		return lattice.calc_action();
	});

	const Measurement<int64_t> diffAction = measure_execution(100, [&] -> double {
		return lattice.calc_diff_action(1, 0);
	});

	return LatticeMeasurement { lattice_size, action, diffAction };
}

void measure_lattice_scaling()
{
	std::vector<size_t> sizes (50);
	std::ranges::generate(sizes, [n = 0] mutable -> int { return n += 200000; });

	std::vector<LatticeMeasurement> measurements (sizes.size());
	std::ranges::transform(sizes, measurements.begin(), [i = 0] (const size_t lattice_size) mutable -> LatticeMeasurement {
		std::cout << "\rMeasure lattice scaling: " + std::to_string(++i) + "/50" << std::flush;
		return measure_lattice(lattice_size);
	});

	write_measurement_output(measurements);
	std::cout << std::endl;
}

int main()
{
    std::filesystem::create_directory("output");

	measure_lattice_scaling();

	return 0;
}