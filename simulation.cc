#include <valarray>
#include <sstream>
#include <iostream>
#include <cmath>

#include "myrandom.h"
#include "simulation.h"

/** \brief 		Default constructor for class Simulation. This constructor first initializes
*				the members of the class. The prices private member is a vector of valarrays.
*				In other words, each element of the vector is a valarray. Each element in the
*				vector corresponts to a point in time, ts, and each element in the valarray
*				is the vaalue of that particular simulation path at time ts. If you want N
*				simulations, then each valarray should have N elements with initial value 
*				params.S0 - the spot price of the stock at time t=0. The number of simulations,
*				N, is the number of columns, and the number of time steps, num_ts, is the number
*				of rows. 
*   \param 		p - Reference to our parameters (strike, vol, time, etc.)
*   \param      num_sims - The number of Monte Carlo simulations
*   \param      num_ts - The number of time steps
* 	\return		Default constructor never has a return type.
*
*/
Simulation::Simulation(Parameters &p, int num_sims, int num_ts) : num_timesteps{num_ts}, params{p}, N{num_sims},
                                                                  delta_t{(params.T - params.t0) / num_timesteps} {

    /* Create space for (num_ts+1) points in time (valarrays), where each valarray has initial value params.S0
     * (initial spot price). */
    prices_.resize(num_ts + 1);

    for (auto i = 0; i < num_ts + 1; ++i) {
        prices_[i].resize(N);
    }

    prices_[0] = params.S0;
}

/** \brief 		This function inserts a valarray at a given timestep n. To do this, the
*				function first declares an iterator for the vector of valarrays. It then
*				iterates to the timestep n in the array of prices and inserts a new 
*				simulated path at a given time step. 
*   \param 		vals - The valarray we want to insert
*   \param      n - The given time step for val to be inserted.
* 	\return		Default constructor never has a return type.
*
*/
void Simulation::insert_valarray_at_step(std::valarray<double> vals, int n) {

//    prices_.insert (prices_.begin()+n, vals);   // equivalent to prices_[n] = vals;

    prices_[n] = vals;

}

/**  \brief     This function returns a reference to a valarray at time step n. Whenever
*               this function is called inside main, it will return a valarray which
*               contains the simulated path at that time step.
*   \param      n . The time-step for requested simulated path.
*   \return     valarray<double>& . A valarray of doubles, the values of the simulated path
*               at time step n.
*
*/
std::valarray<double> &Simulation::get_valarray_at_step(int n) {

//    return prices_.at (n);      // equivalent to return prices_[n];
    return prices_[n];
}



/* ----------------------------------- Euler-Maruyama method ----------------------------------- */

/** \brief 		This function is used for the Euler-Maruyama scheme. The dynamics of the Euler-
*				Maruyama scheme follow eq. 5. 
*   \param 		p . N . ts . rng . p is a reference to a Parameters structure. N is the number
*				of simulation paths and ts is the number of steps between t0 and T. rng is a 
*				reference to an object of type Gaussan_RNs which contains N x ts random variates
*				from the standard normal distribution.
*/
Euler_Maruyama::Euler_Maruyama(Parameters &p, int N, int ts, const Gaussian_RNs &rng)
        : Simulation{p, N, ts} {

    std::cout << "Constructor for Euler-Maruyama scheme constructing." << '\n';
    int prev_idx{0};
    double root_delta_t{std::sqrt(delta_t)};
    std::valarray<double> rans(N);
    double deterministic = 1 + (params.mu * delta_t);

    for (int idx = 1; idx <= num_timesteps; ++idx) {
        std::generate(std::begin(rans), std::end(rans), rng);
        rans *= (root_delta_t * params.sigma);
        rans += deterministic;
        insert_valarray_at_step(get_valarray_at_step(prev_idx) * rans, idx);
        prev_idx = idx;
    }

}

/* ----------------------------------- Exact method method ----------------------------------- */

/** \brief 		This function is used for the Exact_path scheme. The dynamics of the Exact
*				scheme follow Eq. 7. 
*   \param 		p . N . ts . rng . p is a reference to a Parameters structure. N is the number
*				of simulation paths and ts is the number of steps between t0 and T. rng is a 
*				reference to an object of type Gaussan_RNs which contains N x ts random variates
*				from the standard normal distribution.
*/
Exact_path::Exact_path(Parameters &p, int N, int ts, const Gaussian_RNs &rng)
        : Simulation{p, N, ts} {

    std::cout << "Exact_path constructor constructing.\n";
    int prev_idx{0};                                            //< Initialize previous index id
    double root_delta_t{std::sqrt(delta_t)};                    //< Square root of delta_t
    std::valarray<double> rans(N);                              //< Initialize valarray of size N (# simulations)
    double deterministic = (params.mu -
                            0.5 * params.sigma * params.sigma) *
                           delta_t;                             //< Deterministic part of exponential

    for (int idx = 1; idx <= num_timesteps; ++idx) {

        std::generate(std::begin(rans), std::end(rans), rng);     //< lhs is now z (Gaussian variates)
        rans *= root_delta_t * params.sigma;                            //< lhs now z * root_delta_t * sigma
        rans += deterministic;                                          //< lhs now z*root_delta_t * sigma + deterministic
        rans = std::exp(rans);                                          //< lhs now all raised to exponential

        // At t=0, the path is just S0. So we input our simulation path at t=1
        insert_valarray_at_step(get_valarray_at_step(prev_idx) * rans, idx);    //< insert S0*everything_else at t=1.
        prev_idx = idx;     //< prev_idx = 1
    }
}

/* ----------------------------------- Milstein method ----------------------------------- */

/** \brief 		This function is used for the Milstein scheme. The dynamics of the Milstein
*				scheme follow Eq. 6. 
*   \param 		p . N . ts . rng . p is a reference to a Parameters structure. N is the number
*				of simulation paths and ts is the number of steps between t0 and T. rng is a 
*				reference to an object of type Gaussan_RNs which contains N x ts random variates
*				from the standard normal distribution. 
* 	\return		Default constructor never has a return type.
*
*/
Milstein::Milstein(Parameters &p, int N, int ts, const Gaussian_RNs &rng)
        : Simulation{p, N, ts} {

    std::cout << "Constructor for Milstein scheme constructing." << '\n';

    int prev_idx{0};
    double root_delta_t{std::sqrt(delta_t)};
    std::valarray<double> rans(N);
    double sigma_component = 0.5 * (params.sigma * params.sigma);

    for (int idx = 1; idx <= num_timesteps; ++idx) {
        std::generate(std::begin(rans), std::end(rans), rng);
        rans *= ((params.sigma * root_delta_t) + (rans * sigma_component * delta_t));
        rans += 1 + delta_t * (params.mu - sigma_component);
        insert_valarray_at_step(get_valarray_at_step(prev_idx) * rans, idx);
        prev_idx = idx;
    }
}

