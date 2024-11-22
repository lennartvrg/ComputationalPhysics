#include "lattice_scaling_result.h"

LatticeScalingResult::LatticeScalingResult(const std::size_t lattice_size, const Experiment<int64_t> action, const Experiment<int64_t> diff_action) : lattice_size(lattice_size), action(action), diff_action(diff_action)
{ }
