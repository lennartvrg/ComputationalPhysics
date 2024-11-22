#ifndef LATTICE_H
#define LATTICE_H

#include <cstddef>
#include <generator>

#include "lattice_observable.h"

/**
 * Represents a generic lattice and declares all methods needed for Metroplis-Hastings Monte Carlo methods.
 */
class Lattice {
public:
	/**
	 * Instantiates a new lattice with the given coupling constant j and magnetic field strength h
	 */
    Lattice(const double beta, const double j, const double h) : beta(beta), j(j), h(h) {};
    virtual ~Lattice() = default;


	/**
	 * Flips the spin at index i.
     */
    virtual void flip_spin(size_t i) = 0;

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
	 * Calculates the action difference given the difference of energy and magnetization.
	 */
	[[nodiscard]] double action_diff(double diff_energy, double diff_magnetization) const;

	/**
	 * Calculates the acceptance probability if one was to flip the spin at index i.
	 */
    [[nodiscard]] double acceptance(size_t i) const noexcept;

	/**
	 * Calculates the acceptance probability given the difference of energy and magnetization
	 */
	[[nodiscard]] double acceptance(double diff_energy, double diff_magnetization) const;

	/**
	 * Performs a single lattice sweep and calculates the acceptance ratio for every lattice site and flips
	 * the spin of the site if the acceptance ration is greater than a random number [0, 1].
	 */
	std::generator<LatticeObservable> sweeps();

	/**
	 * Performs the metropolis hastings simulation with the given number of sweeps for a given
	 * external magnetic field h. Returns the mean observable values per spin.
	 *
	 * @param num_sweeps The number of sweeps made in total.
	 * @return The mean observable values.
	 */
	LatticeObservable metropolis_hastings(size_t num_sweeps);

protected:
	/**
	 * The inverse temperature, coupling constant j and the magnetic field strength h.
	 */
    double beta, j, h;

	/**
	 * The current observable values
	 */
	LatticeObservable current;
};

#endif //LATTICE_H
