
#ifndef VECTOR_FRAME_HPP
#define VECTOR_FRAME_HPP

#include <SFML/Graphics/CircleShape.hpp>

#include <vector>
#include <other/Coordinate.hpp>
#include <graphics/Color.hpp>

struct VectorRenderStyle {
    VectorRenderStyle()
    : color(Color::Black) {}
    VectorRenderStyle(const Color& color_)
    : color(color_) {}

    Color color;
};

struct VectorRenderPoint {

    VectorRenderPoint() {}
    VectorRenderPoint(const Coordinate& center_point, double diameter_size, const VectorRenderStyle& point_style)
    : position(center_point),
      size(diameter_size),
      style(point_style)
    {}

    Coordinate position;
    double size;
    VectorRenderStyle style;

};
struct VectorFrame {
    VectorFrame() {}

    void add_point(const VectorRenderPoint& point)
    {
        points.push_back(point);
        sf::CircleShape circle;
        circle.setRadius(point.size/2);
        circle.setOrigin(circle.getRadius(), circle.getRadius());
        circle.setPosition(point.position.x, point.position.y);
        circle.setFillColor( sf::Color(
            point.style.color.r,
            point.style.color.g,
            point.style.color.b )
        );
        circles.push_back(circle);
    }

    std::vector<sf::CircleShape>   circles; // Similar to sprites, we're putting it together with the Frame
    std::vector<VectorRenderPoint> points;
};


// Free functions to deal with updating the circle shapes from the VectorFrame and vice versa

// For now, we're just dealing with the points and circles that represent the points.
// If the user wants to rasterize the vectors, there should be a keyboard shortcut
// that rasterizes the vector points to the RenderFrame.

/**
 * index is the index of the circle/point in both
 * the circle
 */
 /*
void update_vector_frame(VectorFrame& vector_frame, const std::vector<sf::CircleShape>& circle_frame, std::size_t index)
{
    vector_frame.points[index].position = Coordinate(circle_frame[i].getPosition().x,
                                              circle_frame[i].getPosition().y);
    vector_frame[index].size = 2.0 * circle_frame[i].getRadius();

    const sf::Color& color = circle_frame[i].getFillColor();
    vector_frame[index].style.color = Color(color.r, color.g, color.b, color.a);
}

*/


#endif // VECTOR_FRAME_HPP



