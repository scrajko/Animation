
#ifndef COORDINATE_HPP
#define COORDINATE_HPP

#include <iostream>


#define COORDINATE_USE_3D  1

/**
 * Rounds a double to its closest int,
 * negative or positive.
 */
int rounding(double n);


#if COORDINATE_USE_3D

/**
 * Class to represent a 2D or 3D coordinate.
 * Can be used as a point or a vector.
 * Incorporated into classes such as BezierCurve.
 *
 * Has overloaded operators to allow
 * multiplying a coordinate by a scalar.
 *   ex: 3*Coordinate(3.2, 1.4) -> Coordinate(9.6, 42.0)
 *
 */
class Coordinate {
  public:

    //default, makes {0, 0, 0}
    Coordinate();
    //2D, makes {x, y}
    Coordinate(double x, double y);
    //3D, makes {x, y, z}
    Coordinate(double x, double y, double z);

    double magnitude_squared() const;
    double magnitude()         const;

    friend std::ostream& operator<<(std::ostream&, const Coordinate&);
    friend std::istream& operator>>(std::istream&, Coordinate&);

    friend Coordinate operator+(const Coordinate&, const Coordinate&);
    friend Coordinate operator-(const Coordinate&, const Coordinate&);

    /// Multiplies a coordinate by a scalar
    /// Useful if the coordinate is being used as a vector
    friend Coordinate operator*(double, const Coordinate&);
    friend Coordinate operator*(const Coordinate&, double);

    /// Divides a coordinate by a scalar
    friend Coordinate operator/(const Coordinate& lhs, double rhs);

    friend bool operator==(const Coordinate&, const Coordinate&);
    friend bool operator!=(const Coordinate&, const Coordinate&);

  public:
    double x;
    double y;
    double z;

    static const Coordinate Default;
};

#else // COORDINATE_USE_3D

class Coordinate {
  public:
    Coordinate();
    Coordinate(double x, double y);

  public:
    double x;
    double y;
};

#endif // COORDINATE_USE_3D


double distance(const Coordinate& a, const Coordinate& b);
double distance_squared(const Coordinate& a, const Coordinate& b);

double abs(const Coordinate&);
double abs_squared(const Coordinate&);

Coordinate rounding(const Coordinate& c);

Coordinate defaultPath(double t); // This shouldn't be here, I don't think.

static const Coordinate Default;

#endif // COORDINATE_HPP

