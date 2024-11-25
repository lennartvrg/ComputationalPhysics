#ifndef LATTICE_2D_H
#define LATTICE_2D_H

#include <lattice.h>
#include <vector>

class Lattice2D final : public Lattice {
public:
	Lattice2D(const size_t lattice_length, const double beta, const double j, const double h) : Lattice(beta, j, h), lattice_length(lattice_length), spins(lattice_length * lattice_length, 1) {
	    current = LatticeObservable(0, j, energy(), magnetization());
	}

	void flip_spin(size_t i) override;
	[[nodiscard]] constexpr size_t num_sites() const noexcept override;

	[[nodiscard]] double energy() const override;
	[[nodiscard]] double energy_diff(size_t i) const override;

	[[nodiscard]] double magnetization() const override;
	[[nodiscard]] double magnetization_diff(size_t i) const override;

	static double magnetization_diff(int8_t old_spin);

private:
	const size_t lattice_length;
	std::vector<int8_t> spins;
};

#endif //LATTICE_2D_H
