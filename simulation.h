#ifndef SIMULATION_H_GV5LHPBM
#define SIMULATION_H_GV5LHPBM
#include <valarray>
#include <sstream>
#include <iostream>
#include <vector>
#include <cmath>
#include <valarray>
#include <iostream>

#include "myrandom.h"

/**
 * \brief Structure to hold parameters for the model to be simulated
 *
 * The values are defaulted here. You don't need to modify for
 * the purpose of Assignment 3
 */
struct Parameters
{
    double t0    = 0; 			//!< Initial Time (usually t=0)
    double T     = 1.0; 		//!< Maturity.  End time. 1Y.
    double S0    = 100; 	    //!< Initial Value/price
    double sigma = 0.2; 	    //!< Volatility
    double mu    = 0.05; 		//!< Drift
};

/**
 * \brief Class to hold information related to a simulation
 */
class Simulation
{
public:
    Simulation (Parameters &params, int num_sims, int num_ts); 			//!< Constructor for Simulation Class
    virtual ~Simulation() {
	    std::cout << "Simulation destructor" << std::endl;
    };

    std::valarray<double>& get_valarray_at_step(int n);
    void insert_valarray_at_step(std::valarray<double> vals, int n);

    const int num_timesteps; //!< Number of time-steps for the simulation

protected:
    Parameters params;
    int N; 				//!< Number of simulated paths to generate
    double delta_t;     //!< timestep. i.e. (T-t0)/num_of_timesteps

private:

    std::vector<std::valarray<double>> prices_;    //!< 2-dimensional valarray. Each element in the vector holds a valarray of simulated values
};

/* ---------------------------------- Euler-Maruyama method ----------------------------------- */

class Euler_Maruyama : public Simulation
{
public:
    Euler_Maruyama (Parameters& p, int N, int ts, const Gaussian_RNs& rng);
    ~Euler_Maruyama() {
	std::cout << "Euler-Maruyama destructor" << std::endl;
    };
};


/* ----------------------------------- Exact method method ----------------------------------- */

/**
 * \brief a Class to create simulation paths using exact solution
 */
class Exact_path : public Simulation
{
public:
    Exact_path (Parameters& p, int N, int ts, const Gaussian_RNs& rng);
    ~Exact_path(){
	std::cout << "Exact_path destructor" << std::endl;
    };
};

/* ------------------------------------- Milstein method ------------------------------------ */

/**
 * \brief a Class to create simulation paths using Milstein scheme
 */
class Milstein : public Simulation
{
public:
    Milstein (Parameters& p, int N, int ts, const Gaussian_RNs& rng);
    ~Milstein(){
	std::cout << "Milstein destructor" << std::endl;
    };

};



#endif /* end of include guard: SIMULATION_H_GV5LHPBM */
