
#ifndef CONNECTION_SHAPE_FRAME_HPP
#define CONNECTION_SHAPE_FRAME_HPP

#include <vector>

#include <SFML/System/Vector2.hpp>


const static std::size_t Num_Quadratic_Bezier_Lines = 12;
const static std::size_t Num_Cubic_Bezier_Lines     = 20;

/**
 * index_list_size should, for now, either be 2, 3, or 4!
 *  Find a way to use this in PointConnection class?
 */
std::size_t connection_shape_size(std::size_t index_list_size)
{
    static std::size_t cs_size[] = {
        2,                          // 2 - Line
        Num_Quadratic_Bezier_Lines, // 3 - Bezier2
        Num_Cubic_Bezier_Lines      // 4 - Bezier3
    };
    return cs_size[index_list_size - 2];
}

/**
 * Calculates the intermediate points for the lines,
 * for rendering purposes (thus depends on SFML).
 * Resizes & changes vertices!
 */
void make_quadratic_bezier_lines(std::vector<sf::Vertex>& vertices,
                                 const sf::Vector2f& start,
                                 const sf::Vector2f& control,
                                 const sf::Vector2f& end)
{
    vertices.resize(Num_Quadratic_Bezier_Lines+1);
    for (std::size_t i = 0; i <= Num_Quadratic_Bezier_Lines; i++)
    {
        const double t = i / static_cast<double>(Num_Quadratic_Bezier_Lines);
        vertices[i] = sf::Vector2f(
                (1.0 - t) * ((1.0-t)*start.x + t*control.x)
                     + t  * ((1.0-t)*control.x + t*end.x),
                (1.0 - t) * ((1.0-t)*start.y + t*control.y)
                     + t  * ((1.0-t)*control.y + t*end.y)
        );
    }
}

void make_cubic_bezier_lines(std::vector<sf::Vertex>& vertices,
                             const sf::Vector2f& start,
                             const sf::Vector2f& control1,
                             const sf::Vector2f& control2,
                             const sf::Vector2f& end)
{
    vertices.resize(Num_Cubic_Bezier_Lines+1);
    for (std::size_t i = 0; i <= Num_Cubic_Bezier_Lines; i++)
    {
        const double t = i / static_cast<double>(Num_Cubic_Bezier_Lines);
        vertices[i] = sf::Vector2f(
            (1.0 - t)*(1.0 - t)*(1.0 - t)*start +
          3*(1.0 - t)*(1.0 - t)*t*control1      +
          3*(1.0 - t)*t*t*control2              +
                      t*t*t*end
        );
    }
}


std::vector<Coordinate> generate_point_list_bezier2(const Coordinate& start,
                                                    const Coordinate& control,
                                                    const Coordinate& end)
{
    std::vector<Coordinate> ret;
    return ret;
}


/**
 * TODO: Size for if it's a bezier curve should be set here as well!
 */
class ConnectionShape {
  using Point = sf::Vector2f;
  public:
    ConnectionShape() {}
    /**
     * frame - current frame from scene being worked on.
     * index_list - indices that make up the current connections being worked on,
     *              not the points themselves!!!
     * index_list[i] <=> frame.points[index_list[i]]
     */
    ConnectionShape(const Frame& frame, const std::vector<IndexType>& index_list);
    std::vector<sf::Vertex> vertices;
};
ConnectionShape::ConnectionShape(const Frame& frame, const std::vector<IndexType>& index_list)
: vertices(index_list.size()) // TODO: Needs to be more than this if it's a Bezier curve!
{
    if (index_list.size() < 2) std::cout << "ERRORRRR" << std::endl;

    if (index_list.size() == 2)
    {
        // Line:
        for (std::size_t i = 0; i < index_list.size(); i++)
        {
            vertices[i] = sf::Vector2f(frame.points[index_list[i]].x,
                                       frame.points[index_list[i]].y);
        }
    }
    else if (index_list.size() == 3)
    {
        // Quadratic Bezier:
        Point start   (frame.points[index_list[0]].x,
                       frame.points[index_list[0]].y);
        Point control (frame.points[index_list[1]].x,
                       frame.points[index_list[1]].y);
        Point end     (frame.points[index_list[2]].x,
                       frame.points[index_list[2]].y);
        make_quadratic_bezier_lines(vertices, start, control, end);
    }
    else if (index_list.size() == 4)
    {
        // Cubic Bezier:
        Point start   (frame.points[index_list[0]].x,
                       frame.points[index_list[0]].y);
        Point control1(frame.points[index_list[1]].x,
                       frame.points[index_list[1]].y);
        Point control2(frame.points[index_list[2]].x,
                       frame.points[index_list[2]].y);
        Point end     (frame.points[index_list[3]].x,
                       frame.points[index_list[3]].y);
        make_cubic_bezier_lines(vertices, start, control1, control2, end);
    }
}

/***************
    IDEAS:

System of animation objects instead of just points.
A file could have a reference to a linked animation_object file.
An animation object has features such as:
 - Origin
 - Translation
 - Rotation
 - Mirroring
 - Stretching
 -  ^--- AKA Scaling -> takes two parameters, scaling in x and scaling in y.

// file001.vec
[first comes points]

base_object my_object.vec
rotation = 34.0
translation = 30.0  200.0

// file002.vec
base_object my_object.vec
rotation = 44.0
translation = 50.0  250.0

Question is... how to store this in the actual program?
Also, other transformations such as stretching

And we're not even talking about *color* yet...
... maybe just keep things as black and white and I can draw them in photoshop later


-------------------

You should have a pointer to an instanced object
... but question is, can you then draw this without copying it?



****************/

struct FrameName {
    std::string local_path;
    std::string name;
    std::string extension;
    std::size_t num_digits;
};




#endif // CONNECTION_SHAPE_FRAME_HPP
