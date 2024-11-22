#ifndef LATTICE_2D_H
#define LATTICE_2D_H

#include <lattice.h>
#include <vector>

class Lattice2D final : public Lattice {
public:
	Lattice2D(const size_t lattice_length, const double beta, const double j, const double h) : Lattice(beta, j, h), spins(lattice_length, std::vector<short>(lattice_length, 1)) {
	    current = LatticeObservable(0, j, energy(), magnetization());
	}

	[[nodiscard]] constexpr size_t lattice_length() const;

	void flip_spin(size_t i) override;
	[[nodiscard]] constexpr size_t num_sites() const noexcept override;

	[[nodiscard]] double energy() const override;
	[[nodiscard]] double energy_diff(size_t i) const override;

	[[nodiscard]] double magnetization() const override;
	[[nodiscard]] double magnetization_diff(size_t i) const override;

	static double magnetization_diff(short old_spin);

private:
	std::vector<std::vector<short>> spins;
};

#endif //LATTICE_2D_H
