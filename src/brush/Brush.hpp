
#ifndef BRUSH_HPP
#define BRUSH_HPP

#include <SFML/Config.hpp> // Uint typedefs
#include <SFML/System/Vector2.hpp>

#include <other/Coordinate.hpp>
#include <other/Common.hpp>
#include <brush/BrushDistance.hpp>
#include <brush/BrushMath.hpp>
#include <other/Misc.hpp> // ImageContainer


// unsigned char is directly compatible with sf::Uint8
using Image = //std::vector<unsigned char>;
              std::vector<sf::Uint8>;

const float RGB_Normalization = (1.0f / 255);

enum class BrushMode : int {
    AlphaCircle,
    CircleAlphaArray, // these all do the same thing
    AlphaCone,
    Rectangle,
    FromImage,
    Eraser
};

class Brush {
  public:

    // TODO: More flexible settings for brushes
    Brush(double brush_radius, const std::string& filename)
    : radius(brush_radius),
      max_radius(brush_radius),
      color(Color::Black),
      mode(BrushMode::FromImage),
      previous_position(),
      distance_alpha_values(std::vector<AlphaByte>{0}),
      image(load_ImageContainer_fromFile(filename))
    {}

    Brush(double brush_radius)
    : radius(brush_radius),
      max_radius(brush_radius),
      color(Color::Black),
      mode(BrushMode::AlphaCircle),
      previous_position(),
      distance_alpha_values(std::vector<AlphaByte>{0})
    {}

    /**
     * Paints the brush to the given image with
     * the center of the brush being at center.
     */
    void paint(Image& image, const Coordinate& center,
               const int ImageWidth, const int ImageHeight) const;

    double getSpacing(double spacing_percent) const
    {
        return spacing_percent * this->radius;
    }

    double                 radius;
    double                 max_radius;

    Color                  color;
    BrushMode              mode;

    // Brush should also contain information about last position and stuff?
    //sf::Vector2i
    sf::Vector2f
    previous_position;

    std::vector<AlphaByte> distance_alpha_values;
    ImageContainer image;

  private:
    /**
     * pixel - current pixel (x, y) being worked on.
     * This is in local coordinates!
     */
    Color pixel_color(const Coordinate& pixel, const Coordinate& center) const;

};


/**
 * Paints the top color onto the existing image at the specified pixel coordinate
 * using the alpha composite algorithm function.
 *
 * "image" means bottom layer.
 *
 * Use in a for loop or whatever if you want to process multiple layers
 */
inline void paint_pixel_with_alpha_composite(std::vector<unsigned char>& image,
  const Color& top, const Coordinate& pixel, unsigned ImageWidth, unsigned ImageHeight)
{
    const std::size_t red_index = RGBA_index(pixel, ImageWidth, ImageHeight);
    Color bottom(image[red_index+0],
                 image[red_index+1],
                 image[red_index+2],
                 image[red_index+3]);
    Color c = alpha_composite(top, bottom);

    const double alpha_adjust = 1.0; //c.a*RGB_Normalization;

    image[red_index+0] = alpha_adjust*c.r;
    image[red_index+1] = alpha_adjust*c.g;
    image[red_index+2] = alpha_adjust*c.b;
    image[red_index+3] = alpha_adjust*c.a;
}

/**
 * Same as above but directly pass in the red_index of the current pixel
 */
inline void paint_pixel_with_alpha_composite(std::vector<unsigned char>& image,
  const Color& top, std::size_t pixel_index, unsigned ImageWidth, unsigned ImageHeight)
{
    const std::size_t red_index = pixel_index;
    Color bottom(image[red_index+0],
                 image[red_index+1],
                 image[red_index+2],
                 image[red_index+3]);
    Color c = alpha_composite(top, bottom);

    const double alpha_adjust = 1.0; //c.a*RGB_Normalization;

    image[red_index+0] = alpha_adjust*c.r;
    image[red_index+1] = alpha_adjust*c.g;
    image[red_index+2] = alpha_adjust*c.b;
    image[red_index+3] = alpha_adjust*c.a;
}

#define ALPHA_CAP_TEST
inline void paint_pixel_with_alpha_composite_brushCap(std::vector<unsigned char>& image, const Color& brush_color,
  const Color& top, const Coordinate& pixel, unsigned ImageWidth, unsigned ImageHeight)
{
    const std::size_t red_index = RGBA_index(pixel, ImageWidth, ImageHeight);
    Color bottom(image[red_index+0],
                 image[red_index+1],
                 image[red_index+2],
                 image[red_index+3]);
    Color c = alpha_composite(top, bottom);

    // TODO: I don't know if this is right due to pre-multiplying vs post-multiplying differences
    // Fix for handling brush when opacity is < 100%.
    // Needs more testing to see if this works correctly.
    #ifdef ALPHA_CAP_TEST
    if (c.a > brush_color.a)
    {
        // --Note the following is a note from a previous implementation error:
        // Partially figured out why this isn't working:
        // top color is NOT the original brush color,
        // it could be the low-opacity very edge of the brush.
        c.a = brush_color.a;
    }
    #endif

    const double alpha_adjust = 1.0; //c.a*RGB_Normalization;

    image[red_index+0] = alpha_adjust*c.r;
    image[red_index+1] = alpha_adjust*c.g;
    image[red_index+2] = alpha_adjust*c.b;
    image[red_index+3] = alpha_adjust*c.a;

}


/**
 * Merges the top_layer onto the bottom layer.
 * the top layer will be zeroed,
 * and the bottom layer will contain the merged colors
 * Assumes both arrays are same size.
 */
inline void merge_layers(RawPixelArray& top_layer, RawPixelArray& bottom_main_layer,
                         unsigned ImageWidth, unsigned ImageHeight, const bool zero_top_layer = true)
{
    for (std::size_t i = 0; i < top_layer.size(); i+=4)
    {
        // merge pixel:
        Color top(top_layer[i+0], top_layer[i+1], top_layer[i+2], top_layer[i+3]);
        paint_pixel_with_alpha_composite(bottom_main_layer, top, i, ImageWidth, ImageHeight);

        // zero top layer to make it invisible
        if (zero_top_layer)
        {
            top_layer[i+0] = 0;
            top_layer[i+1] = 0;
            top_layer[i+2] = 0;
            top_layer[i+3] = 0;
        }
    }
    // Updating top_layer's actual texture so that it zeroes the texture as well
    // has to be done externally.
}


#endif // BRUSH_HPP
