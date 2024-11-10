#ifndef MEASUREMENT_H
#define MEASUREMENT_H

#include <span>
#include <cmath>
#include <numeric>
#include <ostream>
#include <sstream>
#include <type_traits>

template<typename T, std::enable_if_t<std::is_arithmetic_v<T>>* = nullptr>
struct Measurement {
	Measurement() = default;

	explicit Measurement(const std::span<T> & measurements) {
		mean = std::accumulate(measurements.begin(), measurements.end(), 0l) / measurements.size();
		uncertainty = std::sqrt(std::accumulate(measurements.begin(), measurements.end(), T(), [&] (const T sum, const T val) -> T {
			return sum + std::pow(val - mean , 2);
		}) / (measurements.size() - 1));
	};

	friend std::ostream & operator<<(std::ostream & os, const Measurement & measurement) {
		std::stringstream output;
		output << measurement.mean << "," << measurement.uncertainty;
		return os << output.str();
	}

	T mean, uncertainty;
};

#endif //MEASUREMENT_H
