#include "lattice_1d.h"

short Lattice1D::fetch_flip(size_t i) {
    const short old_spin = spins.at(i);
    spins.at(i) *= -1;
    return old_spin;
}

double Lattice1D::calc_energy() const {
    int energy = 0;
    for (size_t i = 0; i < spins.size(); ++i)
    {
        energy += spins.at(i) * spins.at((i + 1) % spins.size());
    }
    return -energy;
}

double Lattice1D::calc_diff_energy(const short old_spin, const size_t i) const {
    return 2 * old_spin * (spins.at((i + 1) % spins.size()) + spins.at((i + spins.size() - 1) % spins.size()));
}

double Lattice1D::calc_magnetization() const {
    int magnetization = 0;
    for (const short spin : spins)
    {
        magnetization += spin;
    }
    return magnetization;
}

double Lattice1D::calc_diff_magnetization(const short old_spin) const {
    return -2 * old_spin;
}
