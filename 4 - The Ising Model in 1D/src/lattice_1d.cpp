#include "lattice_1d.h"

short Lattice1D::fetch_flip(size_t i) {
    const short old_spin = spins.at(i);
    spins.at(i) *= -1;
    return old_spin;
}

size_t constexpr Lattice1D::num_sites() noexcept {
    return spins.size();
}

double Lattice1D::energy() const {
    int energy = 0;
    for (size_t i = 0; i < spins.size(); ++i)
    {
        energy += spins.at(i) * spins.at((i + 1) % spins.size());
    }
    return -energy;
}

double Lattice1D::energy_diff(const size_t i) const {
    return 2 * spins.at(i) * (spins.at((i + 1) % spins.size()) + spins.at((i + spins.size() - 1) % spins.size()));
}

double Lattice1D::magnetization() const {
    int magnetization = 0;
    for (const short spin : spins)
    {
        magnetization += spin;
    }
    return magnetization;
}

double Lattice1D::magnetization_diff(size_t i) const {
    return -2 * spins.at(i);
}
