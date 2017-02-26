
#include "Misc.hpp"

std::size_t RGBA_index(const Coordinate& c, const unsigned Width, const unsigned Height)
{
  #if Y_REVERSED
    return 4 * Width * (Height-1 - rounding(c.y)) + 4 * rounding(c.x);
  #else
    return 4 * Width * rounding(c.y) + 4 * rounding(c.x);
  #endif // BOTTOM_LEFT_ORIGIN
}

/*static*/ void check_for_errors()
{
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR)
        std::cout << "OpenGL error: " << err << std::endl;
}


// LodePNG
void encodeOneStep(const char* filename, const std::vector<unsigned char>& image, unsigned width, unsigned height)
{
  //Encode the image
  unsigned error = lodepng::encode(filename, image, width, height);

  //if there's an error, display it
  if(error) std::cout << "encoder error " << error << ": "<< lodepng_error_text(error) << std::endl;
}
std::vector<unsigned char> decodeOneStep(const char* filename, unsigned& width, unsigned& height)
{
  std::vector<unsigned char> image; //the raw pixels

  //decode
  unsigned error = lodepng::decode(image, width, height, filename);

  //if there's an error, display it
  if(error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;

  //the pixels are now in the vector "image", 4 bytes per pixel, ordered RGBARGBA..., use it as texture, draw it, ...
  return image;
}

Coordinate axis_rotation_preTrig(const Coordinate& point, const Coordinate& axis,
                                 double radians)
{
    double cos_theta = std::cos(radians);
    double sin_theta = std::sin(radians);
    double x_rot = (point.x - axis.x)*cos_theta - (point.y - axis.y)*sin_theta + axis.x;
    double y_rot = (point.x - axis.x)*sin_theta + (point.y - axis.y)*cos_theta + axis.y;
    return Coordinate(x_rot, y_rot);
}
Coordinate axis_rotation_postTrig(const Coordinate& point, const Coordinate& axis,
                                  double cos_theta, double sin_theta)
{
    double x_rot = (point.x - axis.x)*cos_theta - (point.y - axis.y)*sin_theta + axis.x;
    double y_rot = (point.x - axis.x)*sin_theta + (point.y - axis.y)*cos_theta + axis.y;
    return Coordinate(x_rot, y_rot);
}

/**
 * Takes in a double that is in the range [0.0, 255.0].
 * This double represents the RGB color that will be rounded.
 *
 * ex: 128.7 -> will have a 7/10 chance of being 129,
 * and a 3/10 chance of being 128
 */
unsigned char RGB_dithering(double rgb_value)
{
    //using urandn = random_decimal;
    #define urandn random_decimal

    double t = rgb_value - static_cast<unsigned char>(rgb_value);
    if (urandn(engine) >= t)
        return rgb_value;
    else
        return rgb_value + 1.0;
}

ImageContainer load_ImageContainer_fromFile(const std::string& filename)
{
    unsigned width, height;
    RawPixelArray pixels = decodeOneStep(filename.c_str(), width, height);
    return ImageContainer(pixels, width, height);
}
