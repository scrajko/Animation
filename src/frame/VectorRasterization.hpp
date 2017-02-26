
#ifndef VECTOR_RASTERIZATION_HPP
#define VECTOR_RASTERIZATION_HPP

// TODO Move this to a VectorGraphics.hpp?

// [Vector]Connection[Point]? see main.cpp

/**
 * This is more so pseudo-code right now,
 * this is not implemented right now
 */

struct Style {
    Color color;
    Brush brush; // ?
};

struct Connection {
    VectorPoint* point1;
    VectorPoint* point2; // See previous definitions in other project
};

struct VectorPoint {
    Coordinate position;
    //Connection connection;
    Style style;
};

void rasterize(points with connections, pixels, Width, Height)
{
    // For now, dealing with just 2-point linear interpolation:
    // Draw at t = 0
    // Multiple draw between frames
    //   turn this into a function that is called from both this function
    //   and the main()'s function, to save code
    //   but the width/color will be changing, so we might not be able to
    // Draw at t = 1
}



/*
void rasterize_vector_onto_raster(const VectorFrame& vector_frame,
                                  RenderFrame& render_frame)
{
    // Should a VectorFrame also contain the circles that are rendered?
    // We have the sprite in the RenderFrame, so why not?
    Brush brush; // shouldn't need to create a brush each time, draw from a pool?
    for (std::size_t i = 0; i < vector_frame.circles.size(); i++)
    {
        brush.radius = vector_frame.point
        rasterize_circle
    }
}
*/


#endif // VECTOR_RASTERIZATION_HPP
