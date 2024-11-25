#ifndef LATTICE_1D_H
#define LATTICE_1D_H

#include <vector>
#include <lattice.h>

class Lattice1D final : public Lattice {
public:
    Lattice1D(const size_t lattice_size, const double beta, const double j, const double h) : Lattice(beta, j, h), spins(lattice_size, 1) {
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
    std::vector<int8_t> spins;
};
#endif //LATTICE_1D_H
