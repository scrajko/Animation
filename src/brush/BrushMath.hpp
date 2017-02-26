
#ifndef BRUSH_MATH_HPP
#define BRUSH_MATH_HPP

#include <cmath>

#include <other/Common.hpp>
#include <other/Coordinate.hpp>
#include <other/Misc.hpp> // Byte

/*
struct LinePair {
   Line a;
   Line b;
};

*/

template <typename ValueType>
struct ValuePair {

};

struct DoublePair {
    DoublePair(double a_value, double b_value)
    : a(a_value), b(b_value) {}
    double a;
    double b;
};

////////////////////////////
///  3D PLOT PRIMITIVES  ///
////////////////////////////


/**
 * Same thing as linearly lowering alpha
 * as distance from center increases,
 * just written as a function of (x, y).
 *
 * TODO: Test against the other function,
 * to see if it really is exactly the same
 *
 * TODO: If bounds are correctly checked beforehand,
 * we don't have to do the if-statement std::max
 *
 * TODO: Make this return a double t,
 * and have outside function figure out how to interpret the value t
 *
 * TODO: Shifts should be done externally?
 *   Depends on how you want to design it...
 */

double cone(double radius, double height, const Coordinate& center,
            const Coordinate& current_pixel);
Byte cone_alpha(double radius, double height, const Coordinate& center,
                const Coordinate& current_pixel);
double triangular_prism(double base_half_length, double height,
                        const Coordinate& centerline, const Coordinate& p);
double truncated_triangular_prism(double start_half_width, double end_half_width, double height,
                                  double width_t, const Coordinate& centerline, const Coordinate& p);


/**
 * Return value represents t when height is 1.0
 *
    |       |     |       |
 *  [radius][width][radius]
 *   (width doesn't overlap outer radii)
 *
 * p represents current pixel (as in, the one that's being iterated x, y)
 * r0_center is the center of the left circle
 * r0_center + Coordinate(width, 0) is the center of right circle
 */

/*
 * This struct is here because it's easier to calculate
 * when we don't need to worry about the rotation.

 * First value represents the interpolation t for the alpha values,
 * Second value represents the interpolation t for the color values
 */
struct t_pair {
    t_pair(double t_a, double t_w)
    : t_alpha(t_a), t_width(t_w) {}
    double t_alpha; // Alpha value of color (ex: 0.0 if right next to edge of shape)
    double t_width; // color value (ex: 0.5 if half way between the two points of color)
};

t_pair semicircle_rectangle_semicircle(
    const Coordinate& p, const Coordinate& r0_center,
    double r0, double r1, double width, double height);

DoublePair tangency_lengths(
    const Coordinate& A, double radius_A,
    const Coordinate& B, double radius_B);

double circle(const Coordinate& center, double radius,
              double length_from_center);
double circle_squared(const Coordinate& center, double radius,
                      double length_from_center);

t_pair circle_band(
    const Coordinate& circle_A_center,
    double            circle_A_radius,
    double            circle_A_tang_length,
    double            circle_A_tang_value,

    const Coordinate& circle_B_center,
    double            circle_B_radius,
    double            circle_B_tang_length,
    double            circle_B_tang_value,

    double height,
    const Coordinate& current_pixel);


#endif // BRUSH_MATH_HPP
