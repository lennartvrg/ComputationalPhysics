#include "lattice_2d.h"

#include <cstdlib>
#include <ranges>

void Lattice2D::flip_spin(const size_t i) {
	const auto [col, row] = std::div(static_cast<int>(i), static_cast<int>(lattice_length));
	spins.at(row * lattice_length + col) *= -1;
}

constexpr size_t Lattice2D::num_sites() const noexcept {
	return spins.size();
}

double Lattice2D::energy() const {
	int energy = 0;
	for (const size_t i : std::views::iota(static_cast<size_t>(0), num_sites())) {
		const auto [col, row] = std::div(static_cast<int>(i), static_cast<int>(lattice_length));
		energy += spins.at(row * lattice_length + col) * (
				spins.at(row * lattice_length + (col + 1) % lattice_length) +
				spins.at((row + 1) % lattice_length * lattice_length + col)
			);
	}
	return -j * energy;
}

double Lattice2D::energy_diff(const size_t i) const {
	const auto [col, row] = std::div(static_cast<int>(i), static_cast<int>(lattice_length));
	return 2 * j * spins.at(row * lattice_length + col) * (
		       spins.at(row * lattice_length + (col + 1) % lattice_length) +
		       spins.at(row * lattice_length + (col + lattice_length - 1) % lattice_length) +
		       spins.at((row + 1) % lattice_length * lattice_length + col) +
		       spins.at((row + lattice_length - 1) % lattice_length * lattice_length + col)
	       );
}

double Lattice2D::magnetization() const {
	int magnetization = 0;
	for (const int8_t spin : spins) {
		magnetization += spin;
	}
	return magnetization;
}

double Lattice2D::magnetization_diff(const size_t i) const {
	const auto [col, row] = std::div(static_cast<int>(i), static_cast<int>(lattice_length));
	return magnetization_diff(spins.at(row * lattice_length + col));
}

double Lattice2D::magnetization_diff(const int8_t old_spin) {
	return -2 * old_spin;
}
