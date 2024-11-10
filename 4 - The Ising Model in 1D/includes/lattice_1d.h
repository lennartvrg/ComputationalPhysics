#ifndef LATTICE_1D_H
#define LATTICE_1D_H

#include <vector>
#include <execution>

#include "lattice.h"

class Lattice1D final : public Lattice {
public:
    Lattice1D(const size_t lattice_size, const double j, const double h) : Lattice(j, h), spins(lattice_size, 1) {}

	short fetch_flip(size_t i) override;

	double calc_energy() const override;
	double calc_diff_energy(short old_spin, size_t i) const override;

	double calc_magnetization() const override;
	double calc_diff_magnetization(short old_spin) const override;

private:
    std::vector<short> spins;
};

#endif //LATTICE_1D_H
