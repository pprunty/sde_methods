#include <map>
#include <valarray>
#include <fstream>
#include <iostream>
#include "empirical.h"

/** \brief      This function takes a valarray of doubles, computes the expected value, or
*               mean of that valarray and returns this value.
*   \param      std::valarray<double>& vals . A reference to a valarray of doubles.
*   \return     avg . The mean of the values in the valarray.
*
*/
double expected_value(const std::valarray<double> &vals) {

    try {
        double avg = vals.sum() / vals.size();
        return avg;
    }
    catch (const std::exception &e) {
        std::cerr << "A standard exception was caught, with message. Problem resizing array: '"
                  << e.what() << "'\n";
        exit(1);
    }
}

/** \brief 		This function takes a valarray of doubles, computes the variance of that
*				valarray, and returns this value.
*   \param 		std::valarray<double>& vals . A reference to a valarray of doubles.
*   \return		var . The variance of the valarray.
*
*/
double variance(const std::valarray<double> &vals) {

    return expected_value(vals * vals) - std::pow(expected_value(vals), 2);

}

/**  \brief     This function takes as input parameter a valarray of doubles and an integer
*               representing the number of bins, which is defaulted to 100. The function uses
*               std::map to generate a density histogram. Given the number of bins, one can calculate
*               each bin size interval, or step size. If a value of the valarray, say vals[1], is inside
*               some bin size interval, then we increment (count) this occurrence inside that interval.
*               To obtain the density, after counting all occurrences inside their respective intervals
*               (bins), we obtain a number which reveal the frequency (count) inside of that interval, and
*               divide this number by the total number of occurrences within each interval (bin).
*   \param      std::valarray<double>& vals - A valarray of doubles
*   \param      num_bins - The number of bins for the histogram
*               bins.
*   \return     std::map<double, double> - A map with intervals first (on the lhs) and density second
*               (on the rhs).
*
*/
std::map<double, double> create_density_hist(const std::valarray<double> &vals, const int num_bins) {

    double range = vals.max() - vals.min();            //< Find range
    double bin_stepsize = range / num_bins;            //< Find width of each bin from (max-min)/number_of_bins

    std::map<double, double> hist;

    for (unsigned int i = 0; i < vals.size(); ++i) {
        double index = (static_cast<int>(vals[i] / bin_stepsize)) * bin_stepsize;
        //index = index/bin_stepsize; //<! Normalize data
        hist[index]++;
    }

    for (auto &p : hist) {
        p.second = p.second / vals.size();
    }

    return hist;        //< Return density histogram to a std::map in main.
}

/** \brief      This function takes the density Histogram we made in the create_density_hist()
*               function, as well as a string corresponding to a filename in main, creates a
*               file of this name, opens it, and then writes the results.
*   \param      std::valarray<double>& in -  The density Histogram mapping
*               calculated in the create_density_hist() function
*   \param      filename - A string which we will use
*               as the name for a file we're going to create, open, and write to.
*
*/
void write_hist_to_file(std::map<double, double> &in, std::string filename) {

    std::cout << "Writing results to file: " << filename << '\n';

    std::ofstream outfile;
    outfile.open(filename);

    if (outfile.is_open()) {
        for (auto const &p : in) {
            outfile << p.first << '\t' << p.second << std::endl;
        }
    } else {
        std::cerr << "Error opening outfile." << '\n';
        exit(1);
    }

}

