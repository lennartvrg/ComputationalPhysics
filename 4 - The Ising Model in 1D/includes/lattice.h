#ifndef LATTICE_H
#define LATTICE_H

#include <cstddef>

class Lattice {
public:
    Lattice(const double j, const double h) : j(j), h(h) {};
    virtual ~Lattice() = default;

    virtual short fetch_flip(size_t i) = 0;
    [[nodiscard]] virtual constexpr size_t num_sites() const noexcept = 0;

    [[nodiscard]] virtual double energy() const = 0;
    [[nodiscard]] virtual double energy_diff(size_t i) const = 0;

    [[nodiscard]] virtual double magnetization() const = 0;
    [[nodiscard]] virtual double magnetization_diff(size_t i) const = 0;

    [[nodiscard]] double action() const;
    [[nodiscard]] double action_diff(size_t i) const;

    [[nodiscard]] double acceptance(size_t i) const noexcept;
private:
    double j, h;
};

#endif //LATTICE_H
