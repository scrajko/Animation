
#include "Randomization.hpp"

#include <ctime>

//const
std::default_random_engine engine(time(nullptr));
//const
std::uniform_real_distribution<double> random_decimal(0.0, 1.0);
//const
std::uniform_real_distribution<double> random_coordinate(-1.0, 1.0);
//const
std::uniform_int_distribution<int>     random255(0, 255);

double randrange(double lower_bound, double upper_bound)
{
    return std::uniform_real_distribution<double>(lower_bound, upper_bound)(engine);
}
