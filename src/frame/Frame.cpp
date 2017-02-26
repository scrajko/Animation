
#include "Frame.hpp"

#include <iostream>
#include <fstream>

/*
PointConnection::PointConnection(std::size_t size)
: index_list(size)
{

}
PointConnection::PointConnection(Connection connection_type)
: index_list(connection_type)
{

}*/
namespace Connection {
    void print_type(const unsigned c)
    {
      switch(c) {
        case Line:
            std::cout << "Line" << std::endl; break;
        case Bezier2:
            std::cout << "Bezier Quadratic" << std::endl; break;
        case Bezier3:
            std::cout << "Bezier Cubic" << std::endl; break;
        default:
            std::cout << "Error - type not implemented." << std::endl; break;
      }
    }
}

PointConnection::PointConnection(const std::vector<IndexType>& indices)
: index_list(indices)
{}
/*
VectorObject::VectorObject(const std::string& filename)
{
    // load object's points
    // load object's connections

    // load object's... objects... uhm...
    while (true)
    {
        VectorObject* curr_obj;
    }
}*/

MainHeading::MainHeading(const std::string& filename)
: width(0), height(0), num_frames(0)
{
    std::ifstream f(filename);
    if (!f)
    {
        std::cout << "Error opening main frame file!" << std::endl;
        return;
    }
    std::cout << "Main Heading loaded." << std::endl;
    f >> width;
    f >> height;
    f >> num_frames;
}

Frame::Frame()
{}

//TODO: save file size by having a frame file say "I'm the same as the previous frame"
Frame::Frame(const std::string& filename)
{
    std::ifstream f(filename);
    if (!f)
    {
        std::cout << "Error opening " << filename << std::endl;
        return;
    }
    std::cout << "Loading frame " << filename << "...";
    std::cout.flush();

    std::size_t points_size;
    f >> points_size;
    std::cout << "  Num points: " << points_size << std::endl;
    points.resize(points_size);
    connections.reserve(points_size); // there *can* be more connections than points

    // Point list:
    for (std::size_t i = 0; i < points.size(); i++)
    {
        f >> points[i].x;
        f >> points[i].y;
    }

    // Connection List:
    while (true)
    {
        unsigned type = Connection::Terminator;
        f >> type;
        if (type == Connection::Terminator || !f)
            break;
        Connection::print_type(type);

        // Add connection:
        std::vector<IndexType> connection_indices(type);
        for (std::size_t i = 0; i < type; i++)
        {
            f >> connection_indices[i];
        }
        connections.push_back(PointConnection(connection_indices));
        // 2 points -> Line
        // 3 points -> Quadratic Bezier
        // 4 points -> Cubic Bezier
        //connections.push_back(PointConnection(type, point_a, point_b);
    }

    #if 0
    // Object List:
    while (true)
    {
        std::string object_name;
        f >> object_name;
        if (object_name == "end")
            break;

        //VectorObject f_obj(object_name);
        // Note: This is a bit recursive in nature...
        // A VectorObject could be composed of
        // an arbitrary amount of nested VectorObjects...
        // Which of course isn't possible programmatically/logically.

        // Solution?:
        // VectorObject contains:
        // * Its points / connections
        // * Pointers to other vector objects that it's composed of.
    }
    #endif

    std::cout << " done." << std::endl;
}

/*

User clicks ->
       if click matches one of the points,
       record index that the point is.
User clicks on another point ->
       record second index

    Then ask user which connection type they want between the two points,
    and add it to the Frame object.
*/

/*
ex usage:
if (point_connection[i].connection == Connection::Line)
{
   // Determine the line made
}
else if (point_connection[i].connection == Connection::Bezier2)
{
   // Determine the curve made
   // ... Need a place to store this data, whether it's a function or a list of points.
}
*/

/*
struct PointTimeConnection {
    List of points;
    list of point connections
};

Animation has a list of PointTimeConnections: [ index representing the frame # ]

If two points are interpolated in time together,
  it simply overwrites the points in the frames that are concerned.

Two types of interpolation: There's spatial (space), and there's temporal (time)

Every animation has a list of frames:
    Every frame has a list of points #since in one frame you might want a different number
    Every frame has a list of PointConnections:

...but you also want a way of keeping track of certain points over time,

if interpolate(point at time A, point at time B)
{
    for every point (at P) in between time A and time B
    {

    }
}
*/
