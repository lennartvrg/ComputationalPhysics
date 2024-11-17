#ifndef EXPERIMENT_H
#define EXPERIMENT_H

#include <span>
#include <cmath>
#include <cassert>
#include <iostream>
#include <numeric>
#include <sstream>
#include <type_traits>

/**
 * Represents a single experiment containing multiple independent measurements from which
 * the mean and uncertainty is calculated.
 */
template<typename T, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
struct Experiment {
	/**
	 * Default constructor with zero values for mean and uncertainty.
	 */
	Experiment() = default;

	/**
	 * Instantiates the mean and uncertainty from a list of measurements.
	 */
	explicit Experiment(const std::span<T> & measurements) {
		assert(measurements.size() > 1);
		mean = std::accumulate(measurements.begin(), measurements.end(), T()) / static_cast<T>(measurements.size());
		uncertainty = std::sqrt(std::accumulate(measurements.begin(), measurements.end(), T(), [&] (const T sum, const T val) -> T {
			return sum + std::pow(val - mean , 2);
		}) / static_cast<T>(std::max(static_cast<size_t>(0), measurements.size() - 1)));
	};

	/**
	 * Pipes the mean and uncertainty to the output stream seperated by a comma.
	 * Intended to be used for serializing to a CSV file.
	 */
	friend std::ostream & operator<<(std::ostream & os, const Experiment & measurement) {
		std::stringstream output;
		output << measurement.mean << "," << measurement.uncertainty;
		return os << output.str();
	}

	/**
	 * The mean and uncertainty values.
	 */
	T mean, uncertainty;
};

#endif //EXPERIMENT_H
