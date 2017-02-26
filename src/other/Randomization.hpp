
#ifndef RANDOMIZATION_HPP
#define RANDOMIZATION_HPP

#include <random>

//const
extern
std::default_random_engine engine;
//const
extern
std::uniform_real_distribution<double> random_decimal;
//const
extern
std::uniform_real_distribution<double> random_coordinate;
//const
extern
std::uniform_int_distribution<int>     random255;
//const

double randrange(double lower_bound, double upper_bound);


#endif // RANDOMIZATION_HPP
