/**
 * \file        sde_methods.cc
 * \brief       This program compares two discretisation schemes, Euler-Maruyama and Milstein, with the exact solution
 *              for GBM. The stochastic differential equations (S.D.Es) are solved using Monte Carlo simulation.
 * \author      P.Prunty
 * \version     1.0
 * \date        2020-03-25
 */
#include <valarray>
#include <sstream>
#include <iostream>
#include <fstream>

#include "myrandom.h"
#include "simulation.h"
#include "empirical.h"

int main(void)
{
    const int NUM_SIMS {10'000};
    const int NUM_TIMESTEPS {10};
    const int NUM_BINS {10};
    Parameters params;
    std::stringstream outfile;

	// Create object of random numbers.
    const Gaussian_RNs ran_nums {NUM_SIMS * NUM_TIMESTEPS};

    //params.T=2;  // Modify maturity etc.

    /** Create Simulation objects. **/
	/* A unique_ptr is a smart pointer that owns and manages another object through a pointer
	   and disposes of that object when the unique_ptr goes out of scope. */

	// Exact scheme
  	std::unique_ptr<Simulation> EX1 = std::make_unique<Exact_path>(Exact_path{params, NUM_SIMS, NUM_TIMESTEPS, ran_nums});
    ran_nums.reset_to_start(); 	// Reset the random number object for next scheme to use the same Gaussian variates.

  	// Milstein scheme
   	std::unique_ptr<Simulation> M = std::make_unique<Milstein>(Milstein{params, NUM_SIMS, NUM_TIMESTEPS, ran_nums});
	ran_nums.reset_to_start();	// Reset the random number object for next scheme to use the same Gaussian variates.

	// Euler-Maruyama scheme
	std::unique_ptr<Simulation> EM = std::make_unique<Euler_Maruyama>(Euler_Maruyama{params, NUM_SIMS, NUM_TIMESTEPS, ran_nums});



    // Create histogram of final prices from Exact process
    outfile << "EX_time_" << params.T << "_timesteps_"<< EX1->num_timesteps << ".txt";
    std::map<double, double> hst1 = create_density_hist(EX1->get_valarray_at_step(EX1->num_timesteps), NUM_BINS);
    write_hist_to_file(hst1, outfile.str());
    outfile.str("");   	// Clear stringstream

    outfile << "M_time_" << params.T << "_timesteps_"<<  M->num_timesteps <<".txt";
    std::map<double, double> hst2 = create_density_hist(M->get_valarray_at_step(M->num_timesteps), NUM_BINS);
    write_hist_to_file(hst2, outfile.str());
    outfile.str("");   	// Clear stringstream

    outfile << "EM_time_" << params.T << "_timesteps_"<<  M->num_timesteps <<".txt";
    std::map<double, double> hst3 = create_density_hist(EM->get_valarray_at_step(EM->num_timesteps), NUM_BINS);
    write_hist_to_file(hst3, outfile.str());
    outfile.str("");   	// Clear stringstream



    // Calculate (central) moments of empirical distributions
    std::cout << "\nExpected value Exact: " << expected_value(EX1->get_valarray_at_step(EX1->num_timesteps));
    std::cout << "\nVariance Exact: " << variance(EX1->get_valarray_at_step(EX1->num_timesteps));
    std::cout << "\n\n";

    std::cout << "\nExpected value Milstein: " << expected_value(M->get_valarray_at_step(M->num_timesteps));
    std::cout << "\nVariance Exact Milstein: " << variance(M->get_valarray_at_step(M->num_timesteps));
    std::cout << "\n\n";
	
    std::cout << "\nExpected value Euler-Maruyama: " << expected_value(EM->get_valarray_at_step(EM->num_timesteps));
    std::cout << "\nVariance Exact Euler-Maruyama: " << variance(EM->get_valarray_at_step(EM->num_timesteps));
    std::cout << "\n\n";
	
    // Create valarray of log returns at time step 0 for Exact scheme.
    std::valarray<double> log_rets1 {std::log(EX1->get_valarray_at_step(EX1->num_timesteps)/EX1->get_valarray_at_step(0))};
    
    // Create a histogram of log returns for Exact scheme.
    outfile << "EX_time_" << params.T << "_log_rets_at_timestep" << EX1->num_timesteps <<".txt";
    std::map<double, double> hst1_1 = create_density_hist(log_rets1, NUM_BINS);
    write_hist_to_file(hst1_1, outfile.str());
    outfile.str("");   	// Clear stringstream

    // Calculate and print out expected value and variance of the log-returns distribution.
    std::cout << "\nExpected Exact log returns: " << expected_value(log_rets1);
    std::cout << "\nVariance Exact log returns: " << variance(log_rets1);
    std::cout << "\n\n";

    /* No need to clean up memory since we used smart pointers! */

    return 0;
}
