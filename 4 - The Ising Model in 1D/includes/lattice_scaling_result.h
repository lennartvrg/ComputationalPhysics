#ifndef LATTICE_SCALING_RESULT_H
#define LATTICE_SCALING_RESULT_H

#include <experiment.h>

struct LatticeScalingResult {
	LatticeScalingResult() = default;
	explicit LatticeScalingResult(std::size_t lattice_size, Experiment<int64_t> action, Experiment<int64_t> diff_action);

	friend std::ostream & operator<<(std::ostream & os, const LatticeScalingResult & measurement) {
		std::stringstream output;
		output << measurement.lattice_size << "," << measurement.action << "," << measurement.diff_action;
		return os << output.str();
	}

	std::size_t lattice_size;
	Experiment<int64_t> action, diff_action;
};

#endif //LATTICE_SCALING_RESULT_H
