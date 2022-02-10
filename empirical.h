#ifndef EMPIRICAL_H_HHVMOMRI
#define EMPIRICAL_H_HHVMOMRI

#include <map>
#include <valarray>
#include <string>

// Function prototypes
std::map<double, double> create_density_hist(const std::valarray<double>& vals, const int num_bins = 100);
void write_hist_to_file(std::map<double, double>& in, std::string filename);
double variance(const std::valarray<double>& vals);
double expected_value (const std::valarray<double>& vals);


#endif /* end of include guard: EMPIRICAL_H_HHVMOMRI */
