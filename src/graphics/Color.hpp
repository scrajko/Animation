
#ifndef COLOR_HPP
#define COLOR_HPP

#include "Color.hpp"

#include <iostream> // << operator
#include <cstdlib>

namespace rgb {
    const unsigned char Min   = 0;
    const unsigned char Max   = 255;
    const unsigned char Alpha = 255;

    const double Max_double   = 255.0;
}

/*
unsigned char noise()
{
    //return rand()%(static_cast<int>(rgb::Max) + 1);
    return rand()%256;
}*/

/**
 * Class for representing an RGBA color.
 * This class incorporates the alpha value, by default 255.
 */
class Color {
  public:
    /// Default ctor, makes (0, 0, 0, 255)
    explicit Color();

    /// 1-arg ctor, makes grayscale image of the given value
    explicit Color(unsigned char value);
    explicit Color(int value);

    /// 3-arg ctor, assigns the corresponding RGB values
    explicit Color(unsigned char r, unsigned char g, unsigned char b);
    explicit Color(int r, int g, int b);

    /// 4-arg ctor, assigns the corresponding RGBA values
    explicit Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    explicit Color(int r, int g, int b, int a);


    /// 3-arg ctor, expects RGB  values normalized to [0.0, 1.0]
    explicit Color(double r, double g, double b);
    /// 4-arg ctor, expects RGBA values normalized to [0.0, 1.0]
    explicit Color(double r, double g, double b, double a);

    // static data
    static const Color Black;
    static const Color White;

    static const Color Red;
    static const Color Green;
    static const Color Blue;

    static const Color Yellow;
    static const Color Cyan;
    static const Color Magenta;

    static const Color Clear;

  public:
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;

  public:
    Color getInverse() const;

    //std::string hex_str() const;

    //static Color noise();

    friend bool operator==(const Color& lhs, const Color& rhs);
    friend bool operator!=(const Color& lhs, const Color& rhs);

    friend std::ostream& operator<<(std::ostream&, const Color&);
};

///////////////////////////////
///  Non-member functions:  ///
///////////////////////////////

Color inverse(const Color& c);

/**
 * Used to combine two layers of pixels into one.
 * See: http://en.wikipedia.org/wiki/Alpha_compositing
 *
 * TODO: Possibly more efficient variable usage.
 */
Color alpha_composite(const Color& top_layer, const Color& bottom_layer);

/**
 * Linearly interpolate between two colors
 * t = [0.0, 1.0]
 */
Color color_interpolation(const Color& start, const Color& end, double t);



#endif // COLOR_HPP
