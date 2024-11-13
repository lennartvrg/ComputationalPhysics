#include <utils.h>

Measurement<int64_t> measure_execution(const std::function<void()> & lambda, const size_t num_runs) {
	std::vector<int64_t> measurements (num_runs, 0);
	std::generate(std::execution::par, measurements.begin(), measurements.end(), [&] {
		const auto begin = std::chrono::steady_clock::now();
		lambda();

		const std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		return std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
	});
	return Measurement(static_cast<std::span<int64_t>>(measurements));
}

void write_output_csv(const std::span<LatticeMeasurement> measurements, const std::string & file_name, const std::string & headers) {
	std::ofstream output;
	output.open("output/" + file_name + ".csv");

	output << headers << "\n";
	std::ranges::copy(measurements, std::ostream_iterator<LatticeMeasurement>(output, "\n"));
	output.close();
}