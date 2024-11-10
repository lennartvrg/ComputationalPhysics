#include "lattice_measurement.h"

LatticeMeasurement::LatticeMeasurement(const std::size_t lattice_size, const Measurement<int64_t> action, const Measurement<int64_t> diff_action) : lattice_size(lattice_size), action(action), diff_action(diff_action)
{ }
