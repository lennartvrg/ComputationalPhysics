#include "lattice_2d.h"

#include <cstdlib>
#include <ranges>

constexpr size_t Lattice2D::lattice_length() const {
	return spins.size();
}

void Lattice2D::flip_spin(const size_t i) {
	const auto [quot, rem] = std::div(static_cast<int>(i), static_cast<int>(lattice_length()));
	spins.at(quot).at(rem) *= -1;
}

constexpr size_t Lattice2D::num_sites() const noexcept {
	return lattice_length() * lattice_length();
}

double Lattice2D::energy() const {
	int energy = 0;
	for (const size_t i: std::views::iota(static_cast<size_t>(0), num_sites())) {
		const size_t length = lattice_length();
		const auto [quot, rem] = std::div(static_cast<int>(i), static_cast<int>(length));

		energy += spins.at(quot).at(rem) * (
				spins.at((quot + 1) % length).at(rem) +
				spins.at(quot).at((rem + 1) % length)
			);
	}
	return -j * energy;
}

double Lattice2D::energy_diff(const size_t i) const {
	const size_t length = lattice_length();
	const auto [quot, rem] = std::div(static_cast<int>(i), static_cast<int>(length));

	return 2 * j * spins.at(quot).at(rem) * (
		       spins.at((quot + 1) % length).at(rem) +
		       spins.at((quot + length - 1) % length).at(rem) +
		       spins.at(quot).at((rem + 1) % length) +
		       spins.at(quot).at((rem + length - 1) % length)
	       );
}

double Lattice2D::magnetization() const {
	int magnetization = 0;
	for (const std::vector<short> & row: spins) {
		for (const short spin: row) {
			magnetization += spin;
		}
	}
	return magnetization;
}

double Lattice2D::magnetization_diff(const size_t i) const {
	const auto [quot, rem] = std::div(static_cast<int>(i), static_cast<int>(lattice_length()));
	return magnetization_diff(spins.at(quot).at(rem));
}

double Lattice2D::magnetization_diff(const short old_spin) {
	return -2 * old_spin;
}
