
#include "Color.hpp"

const Color Color::Black  (0,     0,   0);
const Color Color::White  (255, 255, 255);

const Color Color::Red    (255,   0,   0);
const Color Color::Green  (0,   255,   0);
const Color Color::Blue   (0,     0, 255);

const Color Color::Yellow (255, 255,   0);
const Color Color::Cyan   (0,   255, 255);
const Color Color::Magenta(255,   0, 255);

const Color Color::Clear  (0,  0,  0,  0);

Color::Color()
: r(0), g(0), b(0), a(255)
{}

Color::Color(unsigned char value)
: r(value), g(value), b(value), a(255)
{}
Color::Color(int value)
: r(value), g(value), b(value), a(255)
{}

Color::Color(unsigned char r_, unsigned char g_, unsigned char b_)
: r(r_), g(g_), b(b_), a(255)
{}
Color::Color(int r_, int g_, int b_)
: r(r_), g(g_), b(b_), a(255)
{}

Color::Color(unsigned char r_, unsigned char g_, unsigned char b_, unsigned char a_)
: r(r_), g(g_), b(b_), a(a_)
{}
Color::Color(int r_, int g_, int b_, int a_)
: r(r_), g(g_), b(b_), a(a_)
{}

Color::Color(double r_, double g_, double b_)
: r(r_ * rgb::Max),
  g(g_ * rgb::Max),
  b(b_ * rgb::Max),
  a(rgb::Max)
{}

Color::Color(double r_, double g_, double b_, double a_)
: r(r_ * rgb::Max),
  g(g_ * rgb::Max),
  b(b_ * rgb::Max),
  a(a_ * rgb::Max)
{}


Color Color::getInverse() const
{
    return Color(rgb::Max - r,
                 rgb::Max - g,
                 rgb::Max - b);
}
Color inverse(const Color& c)
{
    return Color(rgb::Max - c.r,
                 rgb::Max - c.g,
                 rgb::Max - c.b);
}

/*
static Color noise()
{
    return Color(rand()%256, rand()%256, rand()%256);
}*/

bool operator==(const Color& lhs, const Color& rhs)
{
    return lhs.r == rhs.r &&
           lhs.g == rhs.g &&
           lhs.b == rhs.b &&
           lhs.a == rhs.a;
}
bool operator!=(const Color& lhs, const Color& rhs)
{
    return !(lhs == rhs);
}
std::ostream& operator<<(std::ostream& os, const Color& src)
{
    if (src.a == 255)
      os << '(' << static_cast<unsigned>(src.r) << ", "
                << static_cast<unsigned>(src.g) << ", "
                << static_cast<unsigned>(src.b) << ')';
    else
      os << '(' << static_cast<unsigned>(src.r) << ", "
                << static_cast<unsigned>(src.g) << ", "
                << static_cast<unsigned>(src.b) << ", "
                << static_cast<unsigned>(src.a) << ')';
    return os;
}


/**
 * Rounds the value to the nearest integer.
 * Used for rounding when you know the input value is positive
 * (or that it will be rounded to 0, ex: -0.01)
 */
inline static int urounding(double value)
{
    return value + 0.5;
}
Color alpha_composite(const Color& top_layer, const Color& bottom_layer)
{
    // Normalizing values:
    double A_r      =  top_layer.r / rgb::Max_double;
    double A_g      =  top_layer.g / rgb::Max_double;
    double A_b      =  top_layer.b / rgb::Max_double;
    double A_alpha  =  top_layer.a / rgb::Max_double;

    double B_r      =  bottom_layer.r / rgb::Max_double;
    double B_g      =  bottom_layer.g / rgb::Max_double;
    double B_b      =  bottom_layer.b / rgb::Max_double;
    double B_alpha  =  bottom_layer.a / rgb::Max_double;

    double new_alpha = A_alpha + B_alpha * (1.0 - A_alpha);

    double new_r =
      (1.0 / new_alpha) * (A_r * A_alpha + B_r*B_alpha * (1.0 - A_alpha) );
    double new_g =
      (1.0 / new_alpha) * (A_g * A_alpha + B_g*B_alpha * (1.0 - A_alpha) );
    double new_b =
      (1.0 / new_alpha) * (A_b * A_alpha + B_b*B_alpha * (1.0 - A_alpha) );

    // apply rounding into constructor:
    // Note: This makes the double constructors useless for now
    return Color(urounding(new_r     * rgb::Max),
                 urounding(new_g     * rgb::Max),
                 urounding(new_b     * rgb::Max),
                 urounding(new_alpha * rgb::Max));

    //return Color(new_r, new_g, new_b, new_alpha);
}

Color color_interpolation(const Color& start, const Color& end, double t)
{
    // Can cause fence post errors for certain values of t
    // ... could have check for if (start.component == end.component) component = end.component;
    unsigned char r = static_cast<unsigned char>( urounding( (1.0 - t)*start.r + t*end.r ) );
    unsigned char g = static_cast<unsigned char>( urounding( (1.0 - t)*start.g + t*end.g ) );
    unsigned char b = static_cast<unsigned char>( urounding( (1.0 - t)*start.b + t*end.b ) );
    unsigned char a = static_cast<unsigned char>( urounding( (1.0 - t)*start.a + t*end.a ) );
    return Color(r, g, b, a);
}

