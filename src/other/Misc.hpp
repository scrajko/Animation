
#ifndef VA_MISC_HPP
#define VA_MISC_HPP

#include <OpenGL_common_SFML.hpp>
#include <lodepng/lodepng.h>

#include <SFML/System/Vector2.hpp>

#include <other/Coordinate.hpp>
#include <graphics/Color.hpp>
#include <other/Randomization.hpp>

#include <algorithm>
#include <vector>

typedef unsigned char Byte;
typedef std::vector<unsigned char> RawPixelArray;
#define Y_REVERSED 0
/**
 * Coordinate c will be rounded before return
 * Returns the index cooresponding to the first, red index,
 * +0 -> R
 * +1 -> G
 * +2 -> B
 * +3 -> A
 */
std::size_t RGBA_index(const Coordinate& c, const unsigned Width, const unsigned Height);
void check_for_errors();

//LodePNG
void encodeOneStep(const char* filename, const std::vector<unsigned char>& image, unsigned width, unsigned height);
std::vector<unsigned char> decodeOneStep(const char* filename, unsigned& width, unsigned& height);


template <typename T>
sf::Vector2<T> operator*(double factor, const sf::Vector2<T>& rhs)
{
    return sf::Vector2<T>(factor*rhs.x, factor*rhs.y);
}
template <typename T>
sf::Vector2<T> operator*(const sf::Vector2<T>& lhs, double factor)
{
    return sf::Vector2<T>(factor*lhs.x, factor*lhs.y);
}

template <typename T>
double distance(const sf::Vector2<T> a, const sf::Vector2<T>& b)
{
    double dx = b.x - a.x;
    double dy = b.y - a.y;
    return std::sqrt(dx*dx + dy*dy);
}

inline
glm::vec3 operator*(double factor, const glm::vec3& v)
{
    return glm::vec3(factor*v.x,
                     factor*v.y,
                     factor*v.z);
}
inline
glm::vec3 operator*(const glm::vec3& v, double factor)
{
    return glm::vec3(factor*v.x,
                     factor*v.y,
                     factor*v.z);
}
inline
glm::vec4 operator*(double factor, const glm::vec4& v)
{
    return glm::vec4(factor*v.x,
                     factor*v.y,
                     factor*v.z,
                     factor*v.w);
}
inline
glm::vec4 operator*(const glm::vec4& v, double factor)
{
    return glm::vec4(factor*v.x,
                     factor*v.y,
                     factor*v.z,
                     factor*v.w);
}

Coordinate axis_rotation_preTrig(const Coordinate& point, const Coordinate& axis,
                                 double radians);
Coordinate axis_rotation_postTrig(const Coordinate& point, const Coordinate& axis,
                                  double cos_theta, double sin_theta);

Coordinate axis_rotation_preTrig(const Coordinate& point, const Coordinate& axis,
                                 double radians);

unsigned char RGB_dithering(double rgb_value);




struct ImageContainer {

    ImageContainer() : width(0), height(0) {}
    ImageContainer(const std::vector<unsigned char> image_pixels,
                   unsigned image_width,
                   unsigned image_height)
    : pixels(image_pixels),
      width(image_width),
      height(image_height)
    {}

    std::vector<unsigned char> pixels;
    unsigned width;
    unsigned height;
};

ImageContainer load_ImageContainer_fromFile(const std::string& filename);


struct PixelInfo {

    PixelInfo() {}
    PixelInfo(const Coordinate& the_position,
              const Color& the_color)
    : position(the_position),
      color(the_color) {}

    Coordinate position;
    Color color;
};


#endif // VA_MISC_HPP


