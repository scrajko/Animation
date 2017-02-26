
#ifndef PAINT_MODEL_HPP
#define PAINT_MODEL_HPP

#include <vector>

#include <SFML/Graphics/Texture.hpp> // sf::Texture

#include <frame/RenderFrame.hpp>
#include <frame/VectorFrame.hpp>

#include <brush/Brush.hpp>

enum class ToolMode {
   Brush,
   Eraser,
   BrushCircleCutoff, // Above = Drawn regular circle, Below = Nothing or something else custom drawn over mouse.
   // TODO: Calculate custom brush outline (from file or something?)

   /**
    * User clicks on 1st frame,
    * store first point in vector,
    * user clicks on a position in second frame,
    * second point stored,
    * (user can click on a third or fourth frame for bezier interpolation).
    * The point is then interpolated from start frame to end frame, inclusive.
    * This also MOVES the pixels in the current layer over.
    */

   Interpolation,
   Move,
   RectangularSelection,
   Selection, // Used for selecting objects like Vector shapes
   Eyedropper,
   Pan,
   PaintBucket
};

/**
 * struct used to contain info for the Interpolation mode,
 * where its used in an array and the point is interpolated from
 * one frame_index to the other.
 * struct to hold information about a point
 * and the current frame the point is paired with
 */
struct FramePoint {
    using Point = Coordinate;
    FramePoint(int x, int y, int index)
    : point(x, y), frame_index(index)
    {}
    Point point;
    int   frame_index;
};

struct MouseState {
    MouseState()
    : mouse_coord(),
      old_mouse_coord(),
      left_button_pressed(false),
      left_button_just_pressed(false),
      right_button_pressed(false),
      right_button_just_pressed(false)
    {}

    sf::Vector2i mouse_coord;
    sf::Vector2i old_mouse_coord;

    bool left_button_pressed;
    bool left_button_just_pressed;

    bool right_button_pressed;
    bool right_button_just_pressed;
};

/**
 * TODO: Make it snap to nearest pixel
 *
 * primary_corner - set when mouse is initially pressed
 * secondary_corner - continuously set to be the current mouse position
 */
struct RectangularSelectionState {
    RectangularSelectionState()
    : active(false)
    {}
    RenderFrame  floating_selection;
    sf::Vector2f primary_corner;
    sf::Vector2f secondary_corner;
    bool active;

    bool hasActiveSelection() const
    {
        return primary_corner != secondary_corner;
    }
};

struct PanState {
    PanState()
    : origin()
    {}
    PanState(const sf::Vector2f hold_click_origin)
    : origin(hold_click_origin)
    {}
    sf::Vector2f origin;
};

/**
 * TODO: Further divide PaintModel up into
 * the actual data that is saved,
 * and run-time only stuff like copied pixels,
 * the temporary top layer etc.
 */
class PaintModel {

  public:
    PaintModel(unsigned FrameWidth, unsigned FrameHeight);

    unsigned frame_pixel_width;
    unsigned frame_pixel_height;
    std::vector<RenderFrame> raster_frames;
    std::vector<VectorFrame> vector_frames;
    std::size_t frame_index;

    ToolMode tool_mode;
    ToolMode prev_tool_mode; // for saving the state in case of temporary modes

    //RawPixelArray copied_pixels;

    RawPixelArray backup_pixels;
    RawPixelArray pixels_for_redo;

    // layer that is written as user draws
    // merges with main, active layer once user releases brush
    // and then zeroes itself again.

    // Why don't I just make it have a RenderFrame...
    RawPixelArray premerged_brush_layer_pixels;
    sf::Texture   premerged_brush_layer_texture;
    sf::Sprite    premerged_brush_layer_sprite;
    bool temp_layer_painted;

    int undo_frame_index;

    Brush brush;

    //TODO: brush_distance shouldn't be part of the brush's memory itself
    double brush_distance = 0.0;

    std::vector<FramePoint> interpolation_points;

    std::string project_name;

    // How many time frames each drawn frame is:
    std::vector<int> frame_durations;

    // mouse_state is like the Logo or Python turtle graphics when used without real-time user input,
    // user must explicity call onMousePressed/Released().
    MouseState                mouse_state;
    PanState                  pan_state;

    RawPixelArray             copy_state;
    RectangularSelectionState rectangular_selection_state;

};

#endif // PAINT_MODEL_HPP
