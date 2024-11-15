#include <cmath>
#include <algorithm>

#include "lattice.h"

double Lattice::action() const {
    return beta * (energy() - h * magnetization());
}

double Lattice::action_diff(const size_t i) const {
    return beta * (energy_diff(i) - h * magnetization_diff(i));
}

double Lattice::acceptance(const size_t i) const noexcept {
    return std::min(1.0, std::exp(-action_diff(i)));
}
