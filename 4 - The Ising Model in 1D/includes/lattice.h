#ifndef LATTICE_H
#define LATTICE_H

#include <cstddef>

/**
 * Represents a generic lattice and declares all methods needed for Metroplis-Hastings Monte Carlo methods.
 */
class Lattice {
public:
	/**
	 * Instantiates a new lattice with the given coupling constant j and magnetic field strength h
	 */
    Lattice(const double j, const double h) : j(j), h(h) {};
    virtual ~Lattice() = default;


	/**
	 * Flips the spin at index i and returns the magnetization difference.
     */
    virtual double flip_fetch_magnetization_diff(size_t i) = 0;

	/**
	 * Returns the number of spins in the lattice.
	 */
    [[nodiscard]] virtual constexpr size_t num_sites() const noexcept = 0;


	/**
	 * Calculates the total energy of the lattice.
	 */
    [[nodiscard]] virtual double energy() const = 0;

	/**
	 * Calculates the energy difference if one was to flip the spin at index i.
	 */
    [[nodiscard]] virtual double energy_diff(size_t i) const = 0;


	/**
	 * Calculates the total magnetization of the lattice.
	 */
    [[nodiscard]] virtual double magnetization() const = 0;

	/**
	 * Calculates the magnetization difference if one was to flip the spin at index i.
	 */
    [[nodiscard]] virtual double magnetization_diff(size_t i) const = 0;

	/**
	 * Calculates the total action of the lattice.
	 */
    [[nodiscard]] double action() const;

	/**
	 * Calculates the action difference if one was to flip the spin at index i.
	 */
    [[nodiscard]] double action_diff(size_t i) const;

	/**
	 * Calculates the acceptance probability if one was to flip the spin at index i.
	 */
    [[nodiscard]] double acceptance(size_t i) const noexcept;

private:
	/**
	 * The coupling constant j and the magnetic field strnegth h.
	 */
    double j, h;
};

#endif //LATTICE_H
