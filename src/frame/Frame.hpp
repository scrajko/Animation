
#ifndef FRAME_HPP
#define FRAME_HPP

#include <vector>
#include <string>

#include <other/Coordinate.hpp>

using IndexType = unsigned int;

// Is enum class preferable?
namespace Connection {
    const unsigned Line       = 2;
    const unsigned Bezier2    = 3;
    const unsigned Bezier3    = 4;
    const unsigned Function   = 5;
    const unsigned Terminator = 6;

    void print_type(const unsigned c);
}

namespace Header {
    const std::string Points      ( "[Points]"      );
    const std::string Connections ( "[Connections]" );
    const std::string Objects     ( "[Objects]"     );
}

struct PointConnection {
    //PointConnection(std::size_t size);
    //PointConnection(Connection connection_type);
    PointConnection(const std::vector<IndexType>& indices);
    /**
     * Connection type determined by index_list size
     *   2 -> Line
     *   3 -> Quadratic Bezier
     *   4 -> Cubic Bezier
     *   TODO: How to incorporate generalized parametric functions?
     *   TODO: How to incorporate filling?
     */
    std::vector<IndexType> index_list;
    //Connection type;
};


/**
 * Used as an object declared in main,
 * that's then passed around as needed
 * "FrameHeading" instead?
 */
struct MainHeading {
    MainHeading();
    MainHeading(const std::string& filename);
    unsigned width;
    unsigned height;
    unsigned num_frames;
};


/**
 * Formed by reading a frame file,
 * which has a correct indices already generated.
 */
class Frame {
  using Point = Coordinate;

  public:
    Frame();
    Frame(const std::string& filename);

    std::vector<Point>           points;
    std::vector<PointConnection> connections;

};



#endif // FRAME_HPP
