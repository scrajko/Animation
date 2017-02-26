
#ifndef BRUSH_DISTANCE_HPP
#define BRUSH_DISTANCE_HPP

#include <fstream> // both for the load function
#include <iostream>
#include <vector>

/*
 Getting back to the basics...
 For a circular brush, having its opacity be a
 function of distance from the center.

 Possibly testing by rendering a grayscale brush circle to PNG,
 and looking at its 3d view, comparing it to Photoshop's.
*/

/**
 * To differentiate our an unsigned char being a componenent of RGBA,
 * std::vector<AlphaByte> is just an array to represent the alpha values,
 * not to represent an image.
 */
typedef unsigned char AlphaByte;


// Dependent on current color... not efficient
//std::vector<Color> generate_color_array

// Only depends on alpha, other variations are less costly

/**
 * Each circular brush can have a unique distance -> alpha value mapping,
 * independent of the brush's current color.
 * data_size >= 1

 * TODO: Have modifiers for light/dark toning as well.
 * http://stackoverflow.com/questions/141855/programmatically-lighten-a-color
 *
 * TODO: the data_size is redundantly the size of the vector itself, get rid of it, or turn it into a getter()
 * This is deprecated for now until we replace it to allow the light/dark modifiers
 *
 */
struct BrushDistanceMapping {
    BrushDistanceMapping(const std::vector<AlphaByte>& arr, std::size_t datasize)
    : distance_array(arr), data_size(datasize)
    {}
    std::vector<AlphaByte> distance_array;
    std::size_t data_size;
};

/**
 * In this function,
 * each element in the return vector does not represents RGBARGBARGBA,
 * but just the alpha part, AAAAA, without any respect to image dimensions.
 *
 * The following unction only natively works when our radius is exactly the data_size.
 * If the current brush's radius is larger than the data_size, some indices will be repeated.
 * If the current brush's radius is smaller than the data_size, some indices will be skipped.
 * All of this should be accurately linear, which the function distance_value() attempts to do.
 *
 * filename's contents is formatted this example:
 *  8       // data size
 *  255     // number of items must match data size
 *  255
 *  255
 *  255
 *  255
 *  128
 *  64
 *  32
 */
std::vector<AlphaByte> load_alpha_distance_array(const std::string& filename);
unsigned char distance_value(double distance, double radius, const std::vector<AlphaByte>& distance_values);


#endif // BRUSH_DISTANCE_HPP

