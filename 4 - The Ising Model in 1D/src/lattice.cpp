#include "lattice.h"

double Lattice::calc_action() const {
    return j * (calc_energy() - h * calc_magnetization());
}

double Lattice::calc_diff_action(const short old_spin, const size_t i) const {
    return j * (calc_diff_energy(old_spin, i) - h * calc_diff_magnetization(old_spin));
}
