#include "lattice_1d.h"

#include <ranges>

void Lattice1D::flip_spin(const size_t i) {
    spins.at(i) *= -1;
}

constexpr size_t Lattice1D::num_sites() const noexcept {
    return spins.size();
}

double Lattice1D::energy() const {
    int energy = 0;
    for (const size_t i : std::views::iota(static_cast<size_t>(0), spins.size()))
    {
        energy += spins.at(i) * spins.at((i + 1) % spins.size());
    }
    return -j * energy;
}

double Lattice1D::energy_diff(const size_t i) const {
    return 2 * j * spins.at(i) * (spins.at((i + 1) % spins.size()) + spins.at((i + spins.size() - 1) % spins.size()));
}

double Lattice1D::magnetization() const {
    int magnetization = 0;
    for (const short spin : spins)
    {
        magnetization += spin;
    }
    return magnetization;
}

double Lattice1D::magnetization_diff(const size_t i) const {
    return magnetization_diff(spins.at(i));
}

double Lattice1D::magnetization_diff(const short old_spin) {
    return -2 * old_spin;
}
