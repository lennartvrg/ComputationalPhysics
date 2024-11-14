#ifndef LATTICE_RESULT_H
#define LATTICE_RESULT_H

#include <experiment.h>

struct LatticeResult {
	LatticeResult() = default;
	explicit LatticeResult(std::size_t lattice_size, Experiment<int64_t> action, Experiment<int64_t> diff_action);

	friend std::ostream & operator<<(std::ostream & os, const LatticeResult & measurement) {
		std::stringstream output;
		output << measurement.lattice_size << "," << measurement.action << "," << measurement.diff_action;
		return os << output.str();
	}

	std::size_t lattice_size;
	Experiment<int64_t> action, diff_action;
};

#endif //LATTICE_RESULT_H
