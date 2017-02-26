
#include "Coordinate.hpp"

#include <cmath>

const Coordinate Coordinate::Default(0, 0);

int rounding(double n)
{
    return (n >= 0.0)? static_cast<int>(n + 0.5)
                     : static_cast<int>(n - 0.5);
}

double distance(const Coordinate& a, const Coordinate& b)
{
    double dx = b.x - a.x;
    double dy = b.y - a.y;
    return std::sqrt(dx*dx + dy*dy);
}
double distance_squared(const Coordinate& a, const Coordinate& b)
{
    double dx = b.x - a.x;
    double dy = b.y - a.y;
    return dx*dx + dy*dy;
}

Coordinate rounding(const Coordinate& c)
{
    return Coordinate(rounding(c.x), rounding(c.y));
}
Coordinate defaultPath(double t)
{
    return Coordinate(0, 0);
}



#if COORDINATE_USE_3D


Coordinate::Coordinate()
: x(0.0),
  y(0.0),
  z(0.0)
{}

//2D, makes {x, y}
Coordinate::Coordinate(double x, double y)
: x(x),
  y(y),
  z(0.0)
{}

//3D, makes {x, y, z}
Coordinate::Coordinate(double x, double y, double z)
: x(x),
  y(y),
  z(z)
{}

std::ostream& operator<<(std::ostream& os, const Coordinate& src)
{
    if (src.z == 0.0)
        os << '(' << src.x << ", " << src.y << ')';
    else
        os << '(' << src.x << ", " << src.y << ", " << src.z << ')';
    return os;
}
std::istream& operator>>(std::istream& is, Coordinate& src)
{
    is >> src.x >> src.y;
    return is;
}

Coordinate operator+(const Coordinate& lhs, const Coordinate& rhs)
{
    return Coordinate(lhs.x + rhs.x,
                      lhs.y + rhs.y,
                      lhs.z + rhs.z);
}
Coordinate operator-(const Coordinate& lhs, const Coordinate& rhs)
{
    return Coordinate(lhs.x - rhs.x,
                      lhs.y - rhs.y,
                      lhs.z - rhs.z);
}

Coordinate operator*(double lhs, const Coordinate& rhs)
{
    return Coordinate(lhs * rhs.x,
                      lhs * rhs.y,
                      lhs * rhs.z);
}
Coordinate operator*(const Coordinate& lhs, double rhs)
{
    return Coordinate(lhs.x * rhs,
                      lhs.y * rhs,
                      lhs.z * rhs);
}

Coordinate operator/(const Coordinate& lhs, double rhs)
{
    return Coordinate(lhs.x / rhs,
                      lhs.y / rhs,
                      lhs.z / rhs);
}

bool operator==(const Coordinate& lhs, const Coordinate& rhs)
{
    return lhs.x == rhs.x &&
           lhs.y == rhs.y &&
           lhs.z == rhs.z;
}
bool operator!=(const Coordinate& lhs, const Coordinate& rhs)
{
    return lhs.x != rhs.x ||
           lhs.y != rhs.y ||
           lhs.z != rhs.z;
}

#include <cmath>
double abs(const Coordinate& src)
{
    return std::sqrt(src.x * src.x +
                     src.y * src.y +
                     src.z * src.z);
}
double abs_squared(const Coordinate& src)
{
    return src.x * src.x +
           src.y * src.y +
           src.z * src.z;
}

#else // 2D for simpler processing:


#endif // COORDINATE_USE_3D
