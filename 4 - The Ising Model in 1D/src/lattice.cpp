#include "lattice.h"

double Lattice::action() const {
    return j * (energy() - h * magnetization());
}

double Lattice::action_diff(const size_t i) const {
    return j * (energy_diff(i) - h * magnetization_diff(i));
}
