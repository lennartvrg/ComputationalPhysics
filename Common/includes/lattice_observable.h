#ifndef LATTICE_OBSERVABLE_H
#define LATTICE_OBSERVABLE_H

#include <cassert>
#include <sstream>

struct LatticeObservable {
	LatticeObservable() = default;
	LatticeObservable(const size_t sweeps, const double j, const double energy, const double magnetization) : sweeps(sweeps), j(j), energy(energy), magnetization(magnetization) {};

	friend LatticeObservable operator+(const LatticeObservable & sum, const LatticeObservable & rhs) {
		assert(sum.j == rhs.j);
		return { rhs.sweeps, rhs.j, sum.energy + rhs.energy, sum.magnetization + rhs.magnetization };
	}

	LatticeObservable & operator+=(const LatticeObservable & val) {
		assert(j == val.j);
		sweeps = val.sweeps;
		energy += val.energy;
		magnetization += val.magnetization;
		return *this;
	}

	friend LatticeObservable operator/(const LatticeObservable & sum, const size_t & rhs) {
		return { sum.sweeps, sum.j, sum.energy / static_cast<double>(rhs), sum.magnetization / static_cast<double>(rhs) };
	}

	friend std::ostream & operator<<(std::ostream & os, const LatticeObservable & rhs) {
		std::stringstream output;
		output << std::format("{}", rhs.j) << "," << std::format("{}", rhs.sweeps) << "," << std::format("{}", rhs.energy) << "," << std::format("{}", rhs.magnetization);
		return os << output.str();
	}

	size_t sweeps;
	double j, energy, magnetization;
};

#endif //LATTICE_OBSERVABLE_H
