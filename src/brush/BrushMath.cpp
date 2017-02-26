
#include "BrushMath.hpp"


double cone(double radius, double height, const Coordinate& center,
            const Coordinate& current_pixel)
{
    const double shift_x = center.x - current_pixel.x;
    const double shift_y = center.y - current_pixel.y;
    const double c = radius / height;
    return std::max(0.0, -std::sqrt((shift_x * shift_x + shift_y * shift_y) / (c*c)) + height);
}

Byte cone_alpha(double radius, double height, const Coordinate& center,
                const Coordinate& current_pixel)
{
    const double shift_x = center.x - current_pixel.x;
    const double shift_y = center.y - current_pixel.y;
    const double c = radius / height;
    return static_cast<Byte>(
      rounding(std::max(0.0, -std::sqrt((shift_x * shift_x + shift_y * shift_y) / (c*c)) + height))
    );
}

/**
 * Looks like a triangular prism,
 * but actually doesn't depend on the x part
 * (meaning it extends to all reals on the x axis)
 * of the input Coordinate.
 *
 * "base length radius" is misleading since we're not dealing with circles,
 * but it's to differentiate it from being the length of the whole base.
 * It's only the length from the top vertex to the end, AKA half of full length.
 *
 * centerline - kind of misleading, only the y coordinate of this matters
 * the y part of centerline represents the middle of the prism base line.
 */
double triangular_prism(double base_half_length, double height,
                        const Coordinate& centerline, const Coordinate& p)
{
    // Doesn't do "bounds" checking left to right - extends for All Reals
    // That's for the outside if statements to do.
    return std::max(
        0.0, -std::abs((p.y - centerline.y) / (base_half_length / height)) + height
    );

}

/**
 * t represents the value used for interpolation between
 * the start_half_width and the end_half_width
 */
double truncated_triangular_prism(double start_half_width, double end_half_width, double height,
                                  double width_t, const Coordinate& centerline, const Coordinate& p)
{
    double base_half_width = (1.0 - width_t)*start_half_width + width_t * end_half_width;

    return std::max(
        0.0, -std::abs((p.y - centerline.y) / (base_half_width / height)) + height
    );
}

t_pair semicircle_rectangle_semicircle(
    const Coordinate& p, const Coordinate& r0_center,
    double r0, double r1, double width, double height)
{
    if (p.x < r0_center.x)
    {
        // Left Half Cone
        return t_pair(
          cone(r0, height, r0_center, p),
          0.0
        );
    }
    else if (p.x < r0_center.x + width)
    {
        // Triangular Prism
        return t_pair(
          triangular_prism(r0, height, r0_center, p),
          (p.x - r0_center.x) / (width)
        );
    }
    else
    {
        // Right Half Cone
        return t_pair(

          // The following are equivalent:
          cone(r1, height, Coordinate(r0_center.x + width, r0_center.y), p),
          //cone(r1, height, r0_center + Coordinate(width, 0), p),

          1.0
        );
    }
}



/**
 * Calculates the x-distances from centers A and B, respectively,
 * where the endpoints of the lines are tangent to
 * both circles.
 *
 * Assumes circle A and B are both on the same y position
 * (in other words, rotate circle B about A beforehand)
 *
 * To derive again:
 * Start with equation for circle in terms of y, in very general form (with x - x0 and y - y0 shifts)
 * solve first part by saying that Slope[circleA[Ax + tangA]] == Slope[circleB[Bx + tangB]]
 *  - Solve this to put tangB in terms of tangA or vice versa
 * Then, Slope of the line formed from the connection of circleA[tang point A] and circleB[tang point B]
 * must be equal to to Slope[circleA[Ax + tangA], use Assumptions like r > 0 when needed.
 * Be sure to also say that Ay and By == 0, and the solution is quite nice.
 */
DoublePair tangency_lengths(
    const Coordinate& A, double radius_A,
    const Coordinate& B, double radius_B)
{
    // Assume line segment AB is aligned with x-axis
    // Rotations should be done externally.

    //double A_tang_length = (B.x * radius_A + A.x * radius_B) / (2.0 * radius_B);

    double A_tang_length = - radius_A * (radius_A - radius_B) / (A.x - B.x);
    double B_tang_length = (radius_B / radius_A) * A_tang_length;

    return DoublePair(A_tang_length, B_tang_length);
}

/*
 * Given the x value (center.x + the length_from_center),
 * produces the positive y value of the circle
 */

// Circle assumes centered at origin?
// Apply shifting externally.
// NOTICE: I commented out the + center.y because it was messing up
// the other part of the program...
// Just recheck your shit if you want to use this function for other things.
double circle(const Coordinate& center, double radius,
              double length_from_center)
{
    double x = length_from_center + center.x;
    double x_minus_x0 = x - center.x;
    //std::cout << "center:      (" << center.x << ", " << center.y << ")" << std::endl;
    //std::cout << "radius:       " << radius             << std::endl;
    //std::cout << "len from cen: " << length_from_center << std::endl;

    program_assert(radius*radius - x_minus_x0*x_minus_x0 >= 0, "Error in circle(): radius < x value");

    return std::sqrt(radius*radius - x_minus_x0*x_minus_x0); //+ center.y;
}
/** UNUSED IN ACTUAL CODE
 * Similar to circle(), but doesn't call sqrt() for efficiency
 * must be externally handled to provide a meaningful value
 */
double circle_squared(const Coordinate& center, double radius,
                      double length_from_center)
{
    return radius * radius - length_from_center * length_from_center;
}

/*
tangency_points {
    Coordinate A;
    Coordinate B;
    double radius_A;
    double radius_B;
    DoublePair tang_lengths = tangency_lengths(A, B, radius_A, radius_B);

    // Note: Can use ratios to avoid 2nd circle sqrt call
    double yA = circle(A, radius_A, tang_lengths.a);
    double yB = circle(B, radius_B, tang_lenghts.b);

    // yA is start length of interpolation
    // yB is end length of interpolation
    double r = (1.0 - t)*yA + t * yB;
}*/

// A lot of these values are passed in just for the sake of not having to be recalculated
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
    const Coordinate& current_pixel)
{
    double width = circle_B_center.x - circle_A_center.x;

    // @(circle_A_center.x)         -> 0.0
    // @(circle_A_center.x + width) -> 1.0
    double color_mix_t = (current_pixel.x - circle_A_center.x) / (width); // used for the intermediate parts where it's not 0.0 or 1.0

    // Note: tang_lengths are negative if B radius > A radius

    // 4 distinct regions
    // if A > B, the "circle" (but not a cone) region will be 2nd.
    // if B > A, the "circle" (but not a cone) region will be 3rd.
    // best way to handle tis?
    // Either have 4x if-elses with 2x embedded in 2nd,
    // or 5x if-elses and one will be skipped due to tang value stuff

    double cutoff_1 = std::min(circle_A_center.x, circle_A_center.x + circle_A_tang_length);
    //double cutoff_2 = std::min(circle_B_center.x, circle_B_center.x + circle_B_tang_length);

    // TODO: DOUBLE CHECK THIS!!!
    // TODO: Can still see the seam in t values between section A and B

    if (current_pixel.x < cutoff_1)
    {
        // cone, either from [0, radius] (if A > B)
        // or [0, radius - |A_tang_length|] (if A < B)
        return t_pair(
          cone(circle_A_radius, height, circle_A_center, current_pixel), 0.0
        );
    }
    else if (current_pixel.x < circle_A_center.x + circle_A_tang_length)
    {
        // Rounding problem here? Compare @ edges to the other sections

        // If cutoff_1 was the radius,
        // this if statement will be touched.
        // If cutoff was the radius - |A_tang_length|,
        // this if statement will be skipped since it's equal to the first one

        //TODO: Make sure alpha is in [0.0, 1.0], regardless of sign and orientation and stuff
        // Hmm... we're dividing by 0 if circle() is 0
        // Oh but circle() will never be 0 for the give x values
        double alpha_t = std::abs(current_pixel.y - circle_A_center.y) /
                          circle(circle_A_center, circle_A_radius, current_pixel.x - circle_A_center.x);
        alpha_t = 1.0 - alpha_t;
        if (alpha_t < 0.0)
            alpha_t = 0.0;
        //alpha_t = std::abs(alpha_t);
        /*if (alpha_t < 0.0)
            alpha_t = - alpha_t;
        alpha_t = 1.0 - alpha_t; //if (alpha_t < 0)
        if (alpha_t < 0.0)
            alpha_t = 0.0;*/
          //  alpha_t = -alpha_t;
        //if (alpha_t > 1.0)


        // cap alpha values
        return t_pair(alpha_t, color_mix_t);
    }
    /*
    else if (current_pixel.x < circle_B_center.x)
    {
        double begin_position = circle_A_center.x + circle_A_tang_length; // from previous if
        double end_position   = circle_B_center.x; //circle_B_tang_length;
        double t = (current_pixel.x - begin_position) / (end_position - begin_position);
        program_assert(t >= 0.0, "Error in circle_band(): t < 0.0");
        program_assert(t <= 1.0, "Error in circle_band(): t > 1.0");

        double end_value = circle(circle_B_center, circle_B_radius, 0); // no this isn't going to work...
        return t_pair(
          truncated_triangular_prism(circle_A_tang_value, end_value, height, t, circle_A_center, current_pixel),
          color_mix_t //t
        );
    }*/
    else if (current_pixel.x < circle_B_center.x + circle_B_tang_length) //(current_pixel.x < cutoff_2)
    {
        double begin_position = circle_A_center.x + circle_A_tang_length; // from previous if
        double end_position   = circle_B_center.x + circle_B_tang_length;
        double t = (current_pixel.x - begin_position) / (end_position - begin_position);
        program_assert(t >= 0.0, "Error in circle_band(): t < 0.0");
        program_assert(t <= 1.0, "Error in circle_band(): t > 1.0");
        return t_pair(
          truncated_triangular_prism(circle_A_tang_value, circle_B_tang_value, height, t, circle_A_center, current_pixel),
          color_mix_t //t
        );
    }
    else if (current_pixel.x < circle_B_center.x + circle_B_radius)
    {
        // TODO: For now, change it up so we using circle() here as well.
        // But that could cause a problem at the very edge?

        // Start diminishing the height, and interpolate the alpha
        // by circle()
        //new_height[circle_B_center.x + tang_length]     = height;
        //new_height[circle_B_center.x + circle_B_radius] = 0.0;
        double new_height = height*(1.0 - (current_pixel.x                     - (circle_B_center.x + circle_B_tang_length)) /
                                          (circle_B_center.x + circle_B_radius - (circle_B_center.x + circle_B_tang_length)) );
        //std::cout << std::endl;
        //std::cout << (current_pixel.x - (circle_B_center.x + circle_B_tang_length) << std::endl;
        //std::cout << (circle_B_radius - circle_B_tang_length) << std::endl;

        //std::cout << (1.0 - (current_pixel.x - (circle_B_center.x + circle_B_tang_length) /
        //                                    (circle_B_radius - circle_B_tang_length))) << std::endl;
        //new_height = -new_height / 1000;

        double alpha_t = std::abs(current_pixel.y - circle_B_center.y) /
                          circle(circle_B_center, circle_B_radius, current_pixel.x - circle_B_center.x);
        alpha_t = 1.0 - alpha_t; // for large alpha, becomes negative
        if (alpha_t < 0.0)
            alpha_t = 0.0;

        alpha_t = new_height * alpha_t;

        // cap alpha values
        return t_pair(alpha_t, 1.0);//color_mix_t);

        //double new_height = height * circle_B_radius / (circle_B_radius - circle_B_tang_length);
        //std::cout << new_height << std::endl;
        //return t_pair(
        //  cone(circle_B_radius, new_height, circle_B_center, current_pixel), 1.0
        //);
    }
    else
    {
       ///std::cout << "Drawing nothing" << std::endl;
       return t_pair(0.0, 1.0);
    }

    // I'm about to say fuck this bullshit,
    // and just make 1 outside if-else depending on if A > B,
    // and then make 5 inner if's for each scenario
    // ..What if both radius are the same? -> Then tanglength should be zero
    //    but due to rounding errors, this might not be the case,
    //      so perhaps after testing the outside if will be {if, else if, else}

    // TODO: Color interpolation discrepancy
    // It should start interpolating as soon as it passes the radius,
    // not after the radius AND tang_length
    //std::cout << "p.x = " << current_pixel.x << std::endl;
    //std::cout << "tang_length = " << circle_A_tang_length << std::endl;

    /* OLD CODE THAT WORKS EXCEPT FOR PART BETWEEN RADIUS AND TANG_LENGTH */
    #if 0
    if (current_pixel.x < circle_A_center.x  + circle_A_tang_length)
    {
        // Left Part before Tangent

        // TODO: This will actually made the alpha fade
        // for the part between circle center and tang length
        // So basically we need to split up this function in a 5-part piecewise..
        return t_pair(
          cone(circle_A_radius, height, circle_A_center, current_pixel), 0.0
        );

    }
    else if (current_pixel.x < circle_B_center.x + circle_B_tang_length)
    {
        // Note: At this point, if circle_A_tang_length is negative (left of circle origin),
        // double t = (current_pixel.x - circle_A_center.x) / (width);
        // can make t be negative since the current_pixel.x < center.x
        // This erroneous code has been replaced with the following, but note
        // that there's might still be the problem of when to start interpolating
        double begin_position = circle_A_center.x + circle_A_tang_length; // from previous if
        double end_position   = circle_B_center.x + circle_B_tang_length;
        double t = (current_pixel.x - begin_position) / (end_position - begin_position);
        program_assert(t >= 0.0, "Error in circle_band(): t < 0.0");
        program_assert(t <= 1.0, "Error in circle_band(): t > 1.0");
        return t_pair(
          truncated_triangular_prism(circle_A_tang_value, circle_B_tang_value, height, t, circle_A_center, current_pixel),
          t
        );
    }
    else // we're filling in the remainder of B's circle.
    {
        // we make the new height bigger so that it is
        // 1.0 at tangB at the center, but falls off bigger
        // Not ideal, but it should work until
        // I find a better solution
        double new_height = height * circle_B_radius / (circle_B_radius - circle_B_tang_length);
        std::cout << new_height << std::endl;
        return t_pair(
          cone(circle_B_radius, new_height, circle_B_center, current_pixel), 1.0
        );
    }
    #endif
}

