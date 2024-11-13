#ifndef METROPOLIS_RESULT_H
#define METROPOLIS_RESULT_H

#include <experiment.h>

struct MetropolisResult {
	MetropolisResult() = default;
	explicit MetropolisResult(double h, Experiment<double> intensive_magnetization);

	friend std::ostream & operator<<(std::ostream & os, const MetropolisResult & result) {
		std::stringstream output;
		output << result.h << "," << result.intensive_magnetization;
		return os << output.str();
	}

	double h;
	Experiment<double> intensive_magnetization;
};

#endif //METROPOLIS_RESULT_H
