#ifndef UTILS_H
#define UTILS_H

#include <chrono>
#include <execution>
#include <fstream>

#include <experiment.h>

Experiment<int64_t> measure_execution(const std::function<void()> & lambda, size_t num_runs);

template<typename T>
void write_output_csv(const std::span<T> measurements, const std::string & file_name, const std::string & headers) {
	std::ofstream output;
	output.open("output/" + file_name + ".csv");

	output << headers << "\n";
	std::ranges::copy(measurements, std::ostream_iterator<T>(output, "\n"));
	output.close();
}
#endif //UTILS_H
