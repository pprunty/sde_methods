#ifndef RANDOM_H_ZORMJADF
#define RANDOM_H_ZORMJADF
#include <vector>		//< std::vector
#include <memory>		//< 
#include <algorithm>	

/**
 * \brief Class to generate and store normally distributed random numbers
 */
class Gaussian_RNs
{
public:
    Gaussian_RNs(int n);
	double operator()() const;
    void reset_to_start() const;

protected:
    int N_;
    std::vector<double> data_;
    std::shared_ptr<int> cur_idx_ = std::make_shared<int>(0);

};


/**
 *
 *  \brief         A simple class used to hold data for Gaussian variates using
 *                 boost's lagged Fibonacci pseudo random number generator.
 *
 */
class BOOST_Fibonacci : public Gaussian_RNs
{
public:
    BOOST_Fibonacci(int n);
    ~BOOST_Fibonacci() {};
};

/**
 *
 *  \brief         A simple class used to hold data for Gaussian variates using
 *                 Sobol quasi random numbers.
 *
 */
class Sobol : public Gaussian_RNs
{
public:
    Sobol(int n);
    ~Sobol() {};
};

#endif /* end of include guard: RANDOM_H_ZORMJADF */
