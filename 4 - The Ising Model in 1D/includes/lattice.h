#ifndef LATTICE_H
#define LATTICE_H

#include <cstddef>

class Lattice {
public:
    Lattice(const double j, const double h) : j(j), h(h) {};
    virtual ~Lattice() = default;

    virtual short fetch_flip(size_t i) = 0;

    [[nodiscard]] virtual double calc_energy() const = 0;
    [[nodiscard]] virtual double calc_diff_energy(short old_spin, size_t t) const = 0;

    [[nodiscard]] virtual double calc_magnetization() const = 0;
    [[nodiscard]] virtual double calc_diff_magnetization(short old_spin) const = 0;

    [[nodiscard]] double calc_action() const;
    [[nodiscard]] double calc_diff_action(short old_spin, size_t i) const;
private:
    double j, h;
};

#endif //LATTICE_H
