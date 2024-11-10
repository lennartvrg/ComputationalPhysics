#ifndef LATTICE_MEASUREMENT_H
#define LATTICE_MEASUREMENT_H

#include <measurement.h>

struct LatticeMeasurement {
	LatticeMeasurement() = default;
	explicit LatticeMeasurement(std::size_t lattice_size, Measurement<int64_t> action, Measurement<int64_t> diff_action);

	friend std::ostream & operator<<(std::ostream & os, const LatticeMeasurement & measurement) {
		std::stringstream output;
		output << measurement.lattice_size << "," << measurement.action << "," << measurement.diff_action;
		return os << output.str();
	}

	std::size_t lattice_size;
	Measurement<int64_t> action, diff_action;
};



#endif //LATTICE_MEASUREMENT_H
