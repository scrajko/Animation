
#include "Brush.hpp"

void Brush::paint(Image& image, const Coordinate& center,
                  const int ImageWidth, const int ImageHeight) const
{

    // TODO: Is the rounding the best for all situations?
    // Should be int because it could be negative depending on coordinate system
    for (int x = rounding(center.x - radius); x < rounding(center.x + radius); x++)
    {
        for (int y = rounding(center.y - radius); y < rounding(center.y + radius); y++)
        {
            //std::cout << "(" << x << "," << y << ")" << std::endl;
            // Protect against drawing off the page:
            if (x >= ImageWidth || y >= ImageHeight ||
                x < 0           || y < 0)
                continue;

            // Change to just passing in the center?
            // But it does already work so we're leaving it alone for now
            const Color top = pixel_color(Coordinate(x, y), center);

            //paint_pixel_with_alpha_composite
            paint_pixel_with_alpha_composite_brushCap
                (image, this->color, top, Coordinate(x, y), ImageWidth, ImageHeight);

            // TODO: Make better eraser alpha handling

            if (mode == BrushMode::Eraser)
            {
                const std::size_t red_index = RGBA_index(Coordinate(x,y), ImageWidth, ImageHeight);
                image[red_index+0] = 0;
                image[red_index+1] = 0;
                image[red_index+2] = 0;
                image[red_index+3] = 0;
            }

            // If pixel opacity lock is on,
            // blend the RGB colors together but keep the same opacity.
            // Test with Photoshop

            // This not not going to work exactly like I want it to,
            // If 50% opacity blue gets put into 100% opacity green,
            // The resulting color will still have 100% opacity but it won't be green or blue
            // so the color equals check is misleading

            // Other idea is to have a separate in-between pixels sheet
            // Or should opacity be its own sheet of bytes?

            /* Locking Opacity is easy:
            static std::size_t i = 0;
            bool lock_opacity = true;
            if (i++ > 10000000 && lock_opacity)
                c.a = bottom.a;
            */

            // Hard part, trying to preserve opacity between paint calls...
            // maybe need another middleman pixel sheet as temp
        }
    }
}


/**
 * pixel coordinate is with respect to top-left of square??
 * or should it be with respect to center?
 */
Color Brush::pixel_color(const Coordinate& pixel, const Coordinate& center) const
{

    // TODO: Make this unique for each loaded brush!

    // TODO: Figure out how to preserve opacity between paint calls while the mouse is being held down,
    // and later, how to

    // Note: Notice in the SFML Paint program,
    // holding it down makes it look less blurry,
    // but holding it down too log brings noticeable aliasing.

    // TODO: I think you can reduce the redundant "pixel" vs "center/origin" calculations.
    // Coordinate center(pixel.x + radius, pixel.y + radius);

    if (mode == BrushMode::AlphaCircle)
    {
        if (distance_squared(pixel, center) > radius*radius)
            return Color(0, 0, 0, 0);
        else
        {
            // For now, comp is only affecting the alpha channel.
            // Not sure which part of the code is causing the "boxes"
            const double comp = 1.0 - std::sqrt(distance_squared(pixel, center)) / radius;

            program_assert(comp >= 0.0);

            //const double comp = 1.0 - distance_squared(pixel, center) / radius;

            // TODO: Make it work with any color.
            return Color(static_cast<unsigned char>(color.r),
                         static_cast<unsigned char>(color.g),
                         static_cast<unsigned char>(color.b),
                         static_cast<unsigned char>(color.a*comp)); // decreases alpha!
        }
    }
    else if (mode == BrushMode::Rectangle)
    {
        return color;
    }
    else if (mode == BrushMode::AlphaCone)
    {
        if (distance_squared(pixel, center) > radius*radius)
            return Color(0, 0, 0, 0);
        else
        {
            double alpha = cone_alpha(radius, 255, center, pixel) / 255.0;
            return Color(color.r, color.g, color.b,
                         static_cast<Byte>(color.a * alpha));
        }
    }
    else if (mode == BrushMode::CircleAlphaArray)
    {
        if (distance_squared(pixel, center) > radius*radius)
            return Color(0, 0, 0, 0);
        else
        {
            double alpha_comp = distance_value(distance(pixel, center), radius, distance_alpha_values) / 255.f;

            return Color(static_cast<unsigned char>(color.r),
                         static_cast<unsigned char>(color.g),
                         static_cast<unsigned char>(color.b),
                         static_cast<unsigned char>(color.a*alpha_comp));
        }
    }
    else if (mode == BrushMode::FromImage)
    {
        //Scaling from the image
        // Image: [ 0, 0, 0, 255, 255, 255, 0, 0, 0 ] width = 9
        // Dest:  [ _, _, _, _, _] width = 5
        // dest[0] = 0*(9-1) / (5 - 1) =         0
        // dest[1] = 1*(9-1) / (5 - 1) = 8 / 5 = 1
        // dest[4] = 4*(9-1) / (5 - 1) = 8
        //Color pixel_color(const BrushImage& src, double x, double y, double brush_diameter, double);
        {
            // Brush is intended to be circular
            double brush_diameter = radius*2.0;

            double offset = rounding(center.x - radius);

            // problem is probably local pixel.x from top corner of paint vs global x coordinate
            std::size_t pixel_x = (pixel.x - offset) * (image.width  - 1) / (brush_diameter - 1) + 0.5;
            std::size_t pixel_y = (pixel.y - offset) * (image.height - 1) / (brush_diameter - 1) + 0.5;
            std::size_t i = RGBA_index(Coordinate(pixel_x, pixel_y), image.width, image.height);

            //std::cout << "x = " << pixel.x << std::endl;
            //std::cout << "y = " << pixel.y << std::endl;
            std::cout << "x = " << pixel_x << std::endl;
            std::cout << "y = " << pixel_y << std::endl;
            program_assert(i < image.pixels.size(), "index >= size!");

            // Image's greyscale value represents opacity for now.
            // Color is handled by the brush's state
            unsigned char alpha = image.pixels[i];

            return Color(color.r, color.g, color.b, alpha);
        }
    }
    /* Handled in paint() for now
    eise if (mode == BrushMode::Eraser)
    {
        return Color(0, 0, 0, 0);
    }*/
    else
    {
        return Color(255, 0, 255);
    }
}
