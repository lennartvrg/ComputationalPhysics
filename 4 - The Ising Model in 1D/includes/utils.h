#ifndef UTILS_H
#define UTILS_H

#include <chrono>
#include <execution>
#include <fstream>

#include <measurement.h>
#include <lattice_measurement.h>

Measurement<int64_t> measure_execution(const std::function<void()> & lambda, size_t num_runs);

void write_output_csv(std::span<LatticeMeasurement> measurements, const std::string & file_name, const std::string & headers);

#endif //UTILS_H
