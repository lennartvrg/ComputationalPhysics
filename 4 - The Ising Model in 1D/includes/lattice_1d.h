#ifndef LATTICE_1D_H
#define LATTICE_1D_H

#include <vector>
#include <lattice.h>

class Lattice1D final : public Lattice {
public:
    Lattice1D(const size_t lattice_size, const double j, const double h) : Lattice(j, h), spins(lattice_size, 1) {}

	short fetch_flip(size_t i) override;
	size_t constexpr num_sites() noexcept override;

	[[nodiscard]] double energy() const override;
	[[nodiscard]] double energy_diff(size_t i) const override;

	[[nodiscard]] double magnetization() const override;
	[[nodiscard]] double magnetization_diff(size_t i) const override;

private:
    std::vector<short> spins;
};

#endif //LATTICE_1D_H
