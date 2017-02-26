
#include "BrushDistance.hpp"


std::vector<AlphaByte> load_alpha_distance_array(const std::string& filename)
{
    std::size_t data_size;
    std::ifstream f(filename);
    if (!f) {std::cout << "Cannot open " << filename << std::endl;}
    f >> data_size;
    std::vector<AlphaByte> alpha_array(data_size);

    int alpha_value = 0; // Needs to be int for ostream << overload
    int i = 0;
    std::cout << data_size << std::endl;
    while (f >> alpha_value)
    {
        std::cout << "alpha_array[" << i << "] = " << alpha_value << std::endl;
        alpha_array[i] = static_cast<AlphaByte>(alpha_value);
        i++;
    }
    return alpha_array; //BrushDistanceMapping(alpha_array, data_size);
}



/**
 *  0 <= distance <= radius
 */
unsigned char distance_value(double distance, double radius, const std::vector<AlphaByte>& distance_values)
{
    // distance_mapping.data_size - 1 ?
    // for rgb values, you multiple them by 255, not 256, so yeah.

    /// Note: This doesn't do any sort of interpolating between values
    /// Ex: Size = 4 and elements = [255, 255, 255, 0],
    /// It shouldn't go directly from 255 -> 0, it should be interpolated
    /// when pre-casted value is between 2 and 3.
    /// Could use similar algorithm to the dithering but without the randomness.
    ///
    /// ! This can matter even on high data-sizes, for example when the
    /// hardness is high, the drop off slope will be very steep...

    // + 0.5 for rounding
    std::size_t i = static_cast<std::size_t>( (distance_values.size()-1) * distance / radius + 0.5);
    return distance_values[i];
}

// Also could be part of the coloring:
// Increasing or decreasing brightnes/darkness a bit
// http://stackoverflow.com/questions/141855/programmatically-lighten-a-color

