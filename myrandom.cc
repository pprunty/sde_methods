#include <vector>
#include <memory>
#include <random>
#include <algorithm>
#include <iostream>
#include <functional>

// Boost
#include <boost/math/special_functions/erf.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/lagged_fibonacci.hpp>
#include <boost/random/variate_generator.hpp>

#include "myrandom.h"
#include "sobol.h"

/**  \brief     Default constructor for class Gaussian_RNs. This function accepts one
*               integer parameter, namely n. This integer will set the number of Gaussian
*               variates to generate. This function wil use the radom_device to seed
*               the Mersenne Twister rng, and thus, produce a non-deterministic seeded
*               engine. The function then binds the Normal probability density function to
*               this seeded_engine, resizes the private vector data to hold n Gaussian
*               variates, before populating this vector with n Gaussian variates using
*               std::generate.
*   \param      n . The number of random variates generated.
* 	\return     Default constructor never has a return type.
*
*/
Gaussian_RNs::Gaussian_RNs(int n) : N_{n} {

	// Gaussian_RNs reply to being called for request of n Gaussian variates.  
	std::cout << "Constructor for " << N_ << " Gaussian variates constructing." << '\n';

	/* Create a vector of Mersenne Twister state size. */
    std::vector<unsigned int> random_data(std::mt19937_64::state_size);
    std::cout << "Creating vector with Mersenne Twister state-size ["
    << std::mt19937_64::state_size << "]." << '\n';

	std::random_device rand;
    std::generate(std::begin(random_data),
    std::end(random_data), std::ref(rand));
    std::seed_seq seeds(std::begin(random_data),
    std::end(random_data));
    std::mt19937_64 seeded_engine(seeds);

	// Bind Normal probability density function to Mersenne Twister to generate Gaussian variates
	auto gen = std::bind(std::normal_distribution<double>{0,1.0}, seeded_engine);

	// Resize vector to make space for n Gaussian variates.
	data_.resize(N_);

	// Populate data vector with Gaussian variates.
	std::generate(std::begin(data_), std::end(data_), gen);
}


/**  \brief     This function overloads the function call operator for this class. When
*               an object of this type is called as a function object (functor), it should
*               return the next unused Gaussian variate from data. If all the Gaussian
*               variates have been used, the elements of the vector data containing the
*               Gaussian variates is shuffled and the current index is set to zero. Thus,
*               the randomness, or occurrence, of the Gaussian variates is somewhat maintained.
*               The std::shared_ptr here is used for when two or more pieces of code need
*               access to some data, but neither have exclusive ownership.
* 	\return		data[++cur_idx] . The next unused Gaussian variate from data.
*/
double Gaussian_RNs::operator()() const {

	// If all Gaussian variates have been called. The current index reaches the end of the vector. 
	if ( *cur_idx_ == (N_) ) {
		Gaussian_RNs::reset_to_start();     //< Reset cur_idx back to 0
    }
	
	// Return the next unused Gaussian variate from data.
	return data_[(*cur_idx_)++]; //< Postfix rather than prefix, returns data[0] then increments data++

}

/**  \brief     This function will set the current index variable back to 0 everytime
*               that is called so that the next call to operator()() returns the first
*               element of the data again.
*   \param      None.
* 	\return     Void function.
*
*/
void Gaussian_RNs::reset_to_start() const {
	// Set the current index variable back to 0.
	*cur_idx_ = 0;
}



/** \brief          This constructor generates N Gaussian variates using BOOST's lagged Fibonacci
 *                  random number generator. The rng is benchmarked on boost.org as the fastest rng
 *                  to use when generating Gaussian variates. The rng is bound to BOOST's normal
 *                  distribution, which uses Marsaglia's Ziggurat algorithm; an efficient
 *                  means to generate Gaussian variates given a rng.
 *  \param n        The number of random variates
 *
 */
BOOST_Fibonacci::BOOST_Fibonacci(int n) : Gaussian_RNs{n} {

    data_.resize(N_);    //!< Resize vector for N_ variates

    /* Create a vector of Mersenne Twister state size. */
    std::vector<unsigned int> random_data(std::mt19937_64::state_size);
    std::cout << "Creating vector with Mersenne Twister state-size ["
              << std::mt19937_64::state_size << "]." << '\n';

    std::random_device rand;
    std::generate(std::begin(random_data),
                  std::end(random_data), std::ref(rand));
    std::seed_seq seeds(std::begin(random_data),
                        std::end(random_data));
    std::mt19937_64 seeded_engine(seeds);

    static boost::lagged_fibonacci607 rng(seeds);            //!< Seed lagged_fibonacci
    boost::normal_distribution<> std_norm(0, 1);             //!< Declare standard normal dist.
    boost::variate_generator <boost::lagged_fibonacci607,    //!< Bind lagged_fibonacci with normal dist.
    boost::normal_distribution<>> gen(rng, std_norm);

    std::generate(std::begin(data_), std::end(data_), gen);
}

/** \brief          This constructor generates N Gaussian variates using Sobol's quasi random number
 *                  generator. The rng is produces quasi-random low-discrepancy sequences with efficent
 *                  variance.
 *  \param n        The number of random variates
 *  \param seed     The seed for the rng
 *
 */
Sobol::Sobol(int n) : Gaussian_RNs{n} {

    if (N_ > 10'000) {
        std::cerr << "Error. Number of Gaussian variates is too large for Sobol sequence efficacy." << '\n';
        exit(1);
    }

    // Resize vector for N_ variates
    data_.resize(N_);

    // Copy N_ elements of the sobol array into the data_ vector and inverse transform them
    std::transform(std::begin(sobol), std::end(sobol), std::begin(data_), [](const double &x) {
        return boost::math::erf_inv((2 * x) - 1) * std::sqrt(2);
    });

    // Mersenne Twister rng used for shuffling the data
    std::random_device rand;
    std::mt19937 g(rand());

    // Sobol numbers need to be shuffled to achieve good results
    std::shuffle(std::begin(data_), std::end(data_), g);
}
