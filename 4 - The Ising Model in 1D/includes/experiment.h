#ifndef EXPERIMENT_H
#define EXPERIMENT_H

#include <span>
#include <cmath>
#include <cassert>
#include <iostream>
#include <numeric>
#include <sstream>
#include <type_traits>

template<typename T, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
struct Experiment {
	Experiment() = default;

	explicit Experiment(const std::span<T> & measurements) {
		assert(measurements.size() > 1);
		mean = std::accumulate(measurements.begin(), measurements.end(), T()) / static_cast<T>(measurements.size());
		uncertainty = std::sqrt(std::accumulate(measurements.begin(), measurements.end(), T(), [&] (const T sum, const T val) -> T {
			return sum + std::pow(val - mean , 2);
		}) / static_cast<T>(measurements.size() - 1));
	};

	friend std::ostream & operator<<(std::ostream & os, const Experiment & measurement) {
		std::stringstream output;
		output << measurement.mean << "," << measurement.uncertainty;
		return os << output.str();
	}

	T mean, uncertainty;
};

#endif //EXPERIMENT_H
