
#include "Paint_Controller.hpp"

std::string to_string(ToolMode tool_mode)
{
    switch (tool_mode)
    {
      case ToolMode::Brush:
        return "Brush";
      case ToolMode::BrushCircleCutoff:
        return "BrushCircleCutoff";
      case ToolMode::Eraser:
        return "Eraser";
      case ToolMode::Eyedropper:
        return "Eyedropper";
      case ToolMode::Interpolation:
        return "Interpolation";
      case ToolMode::Move:
        return "Move";
      case ToolMode::Pan:
        return "Pan";
      case ToolMode::RectangularSelection:
        return "RectangularSelection";
      case ToolMode::Selection:
        return "Selection";
      default:
        return "Unknown";
    }
}

inline void tool_mode_change(PaintModel& model, ToolMode new_tool_mode)
{
    std::cout << "ToolMode = " << to_string(new_tool_mode) << std::endl;
    model.prev_tool_mode = model.tool_mode;
    model.tool_mode = new_tool_mode;
}

/**
 * The model knows that some of its brush drawing functionality
 * has to depend on a "current position", and a "previous position".
 * What the model doesn't know is that these happen to mouse screen coordinates.
 * The view *could* be implemented as a big blocky array of rectangles, and not just pixels,
 * so you wouldn't want to pass the screen coordinates directly anyway.
 *
 * For now, let's have the controller store the current and previous mouse positions,
 * and have it give that information to the brush's function
 *
 */
void PaintController::handleEvents()
{
    //References: The usual measure used to prevent renaming everything

    Brush& brush = m_model.brush;
    auto& window = m_view.getWindow();

    //auto& frames = m_model.raster_frames;
    //auto& backup_pixels = m_model.backup_pixels;

    auto& tool_mode = m_model.tool_mode;
    auto& view_mode = m_view.view_mode;

    sf::Event event;
    while (window.pollEvent(event))
    {
        // Mouse positions are gotten externally via sf::Mouse::getPosition()
        if (event.type == sf::Event::MouseButtonPressed)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                std::cout << "Left Mouse button Pressed" << std::endl;
                to_mouse_press = true;
            }
            else if (event.mouseButton.button == sf::Mouse::Middle)
            {
                /*
                   need to restructure some stuff
                   I want a "temporary" pan mode that is only activated when the middle mouse button is down
                   need to save the state of the previous mode so that we can switch back to it once the middle
                   mouse button is released
                */
            }
        }
        else if (event.type == sf::Event::MouseButtonReleased)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                to_mouse_release = true;
            }
            else if (event.mouseButton.button == sf::Mouse::Middle)
            {

            }
        }
        else if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Add)
            {
                if (event.key.control)
                    to_zoom_in = true;
                else
                    to_plus = true;
            }
            else if (event.key.code == sf::Keyboard::Subtract)
            {
                if (event.key.control)
                    to_zoom_out = true;
                else
                    to_minus  = true;
            }
            else if (event.key.code == sf::Keyboard::Right)
            {
                to_right = true;
            }
            else if (event.key.code == sf::Keyboard::Left)
            {
                to_left = true;
            }
            else if (event.key.code == sf::Keyboard::A)
            {
                // A is photoshop Selection Tool
                tool_mode = ToolMode::Selection;
            }
            else if (event.key.code == sf::Keyboard::B)
            {
                // Brush mode
                // TODO: Save previous mode to load back into
                // (Rectangle vs AlphaCircle)
                std::cout << "ToolMode = Brush";
                tool_mode = ToolMode::Brush;

                brush.mode = BrushMode::AlphaCircle;
                //brush.mode = BrushMode::CircleAlphaArray;
            }
            else if (event.key.code == sf::Keyboard::C)
            {
                if (event.key.control)
                    to_copy = true;
            }
            else if (event.key.code == sf::Keyboard::E)
            {
                if (event.key.control)
                {
                    to_expand = true;
                }
                else
                {
                    // Erase mode
                    tool_mode = ToolMode::Eraser;
                    brush.mode = BrushMode::Eraser;
                }
            }
            else if (event.key.code == sf::Keyboard::G)
            {
                tool_mode_change(m_model, ToolMode::PaintBucket);
            }
            else if (event.key.code == sf::Keyboard::H)
            {
                tool_mode_change(m_model, ToolMode::Pan); // ToolMode::Hand
            }
            else if (event.key.code == sf::Keyboard::I)
            {
                if (event.key.control)
                {
                    tool_mode = ToolMode::Interpolation;
                }
                else
                {
                    tool_mode = ToolMode::Eyedropper;
                }
            }
            else if (event.key.code == sf::Keyboard::V)
            {
                if (event.key.control)
                    to_paste = true;
                else if (event.key.shift)
                    view_mode = ViewMode::Vector;
                else
                {
                    std::cout << "ToolMode = Move" << std::endl;
                    tool_mode = ToolMode::Move;
                }
            }
            else if (event.key.code == sf::Keyboard::Z)
            {
                if (event.key.control)
                    to_undo = true;
            }
            else if (event.key.code == sf::Keyboard::Y)
            {
                if (event.key.control)
                    to_redo = true;
            }
            else if (event.key.code == sf::Keyboard::R)
            {
                if (event.key.shift)
                    view_mode = ViewMode::Raster;
                else if (event.key.control)
                    to_run = true;
            }
            else if (event.key.code == sf::Keyboard::Insert)
            {
                // If Shift + Insert, it inserts the new frame before the current

                if (event.key.shift)
                    to_insert_pre  = true;
                else
                    to_insert_post = true;
            }
            else if (event.key.code == sf::Keyboard::S)
            {
                // TODO: add "prevent multiple key press" to config
                if (event.key.control)
                {
                    if (event.key.shift)    // Ctrl + Shift + S
                        to_export = true;
                    else if (event.key.alt) // Ctrl + Alt + S
                        to_save_alt = true;
                    else                    // Ctrl + S
                        to_save = true;
                }
            }
            else if (event.key.code == sf::Keyboard::L)
            {
                if (event.key.control)
                    to_load = true;
            }
            else if (event.key.code == sf::Keyboard::Delete)
            {
                to_delete = true;
            }
            else if (event.key.code == sf::Keyboard::M)
            {
                std::cout << "ToolMode = RectangleSelection" << std::endl;
                tool_mode = ToolMode::RectangularSelection;
            }

            else if (event.key.code == sf::Keyboard::Num2)
            {
                if (event.key.control)
                    to_duplicate = true;
            }

        }
        else if (event.type == sf::Event::MouseWheelMoved)
        {
            if (event.mouseWheel.delta > 0)
                to_zoom_in = true;
            else
                to_zoom_out = true;
        }
        else if (event.type == sf::Event::Closed)
        {
            window.close();
        }
    }
}


#include "OnUpdate.hpp"

/**
 * Paints n times inclusively from prev_pos to curr_pos,
 * linearly interpolating the in between steps
 */
inline void paint_brush_line(const sf::Vector2f& prev_pos, const sf::Vector2f& curr_pos,
                      const Brush& brush, RawPixelArray& pixels, std::size_t n,
                      const unsigned FrameWidth, const unsigned FrameHeight)
{
    // inclusive to end position:
    for (std::size_t i = 0; i <= n; i++)
    {
        double t = static_cast<double>(i) / (n);
        sf::Vector2f pos = (1.0-t)*prev_pos + t*curr_pos;
        brush.paint(pixels, Coordinate(pos.x, pos.y), FrameWidth, FrameHeight);
    }
}

// Need to save the last state of the brush's radius/color as well
// to be able to interpolate
/*
struct BrushStyle {
    Color color;
    double diameter;
    double spacing;
};
struct Brush {
    BrushStyle;

   // and RawVectorArray for custom brush options
};*/



/**
 * Refactored: Assumes tool_mode is ToolMode::Brush or ToolMoe::Eraser!
 * Assumptions:
 *     This should only be called *after the mouse as moved.
 *     This assumes mouse is clicked down
 */
void onRasterDrawing(PaintModel& model, sf::Vector2f converted_old_pos,
                                        sf::Vector2f converted_curr_pos)
{
    auto& FrameWidth   = model.frame_pixel_width;
    auto& FrameHeight  = model.frame_pixel_height;
    auto& frames         = model.raster_frames;
    auto& frame_index    = model.frame_index;
    auto& brush          = model.brush;
    auto& brush_distance = model.brush_distance;

    //auto& mouse_coord     = mouse_state.mouse_coord;
    //auto& old_mouse_coord = mouse_state.old_mouse_coord;

    //std::cout << "bd: " << brush_distance << std::endl;

    //auto& pixels = frames[frame_index].pixels;
    auto& pixels = model.premerged_brush_layer_pixels;

    // "Airbrush mode" not currently implemented,
    // so we only draw once per click if cursor doesn't move
    // Interpolate paint between old mouse coord and new
    // Paint if we just clicked, or if we're dragging
    // and have moved (TODO: Have moved beyond the spacing, like how Photoshop does it)

    // Still needs some improvements--when you go fast the stroke looks "thinner" than if you went slow.
    // I think that there's a "gap" of some type

    // Resetting brush.previous_position and brush_distance
    // is now done outside of this function,
    // in an attempt to prevent having to pass left_button_just_pressed!

    if (converted_old_pos != converted_curr_pos)
    {
        // It's continually adding... so we only want it to add
        // one time, after mouse movement
        brush_distance += distance(converted_old_pos, converted_curr_pos);

        //std::cout << "brush_distance = " << brush_distance << std::endl;
    }

    static const double spacing_percentage = 0.10;
    if (brush_distance > brush.getSpacing(spacing_percentage))
    {
        model.temp_layer_painted = true;
        // We see if brush_distance > spacing distance, but not how much greater it is!

        // Calculate how many times we need to draw in-between brushes
        const double double_n = brush_distance / brush.getSpacing(spacing_percentage);
        //std::cout << "n = " << double_n << std::endl;
        const std::size_t n = brush_distance / brush.getSpacing(spacing_percentage);
        //std::cout << "n = " << n << std::endl;

        if (std::isnan(brush_distance) || std::isnan(double_n))
        {
            wait("brush n stuff is NaN. this probably happened because brush size/spacing is 0!");
        }
        if (n >= 0)
        {
            // Why are these the same? TODO: Look over
            //std::cout << "(" << brush.previous_position.x << ", " << brush.previous_position.y << ")" << std::endl;
            //std::cout << "(" << mouse_coord.x << ", " << mouse_coord.y << ")" << std::endl;
        }

        sf::Vector2f prev_pos = brush.previous_position;
        //sf::Vector2f curr_pos = window.mapPixelToCoords(mouse_coord); // now converted_curr_pos
        sf::Vector2f curr_pos = converted_curr_pos;

        if (brush.mode == BrushMode::Eraser)
        {
            // erasing directly to layer
            paint_brush_line(prev_pos, curr_pos, brush, frames[frame_index].pixels, n, FrameWidth, FrameHeight);
            frames[frame_index].texture.update(&frames[frame_index].pixels[0]);
        }
        else // assume brush.mode == BrushMode::Paint
        {
            // writing the "temp layer" before merging
            paint_brush_line(prev_pos, curr_pos, brush, pixels, n, FrameWidth, FrameHeight);
            model.premerged_brush_layer_texture.update(&model.premerged_brush_layer_pixels[0]);
        }

        // Reset:
        brush.previous_position = curr_pos;

        // Change it to this helped, but it's still a bit off...

        brush_distance = (double_n - n)*brush.getSpacing(spacing_percentage);

        // I might be deleting too much from brush_distance:
        //brush_distance -= static_cast<int>(brush_distance); // Leave leftover decimal
    }

}

/**
 * The use of this "Interpolation Mode" is limited at best...
 * I don't think it's correctly updated anymore since it only works
 * if left_button_just_pressed,
 * but I don't think where it's being called from can be accessed
 * if left_button_just_pressed...
 * TODO: Get rid of need for "frame_offset", then bring back into action
 */
#if 0
void onToolModeInterpolation(PaintModel& model, const MouseState& mouse_state)
{
    auto& FrameWidth   = model.frame_pixel_width;
    auto& FrameHeight  = model.frame_pixel_height;
    auto& frames         = model.raster_frames;
    auto& frame_index    = model.frame_index;
    auto& brush          = model.brush;
    auto& interpolation_points = model.interpolation_points;

    // I don't think pixelToCoord mapping is happening here!!!
    const auto& mouse_coord = mouse_state.mouse_coord;
    const auto& left_button_just_pressed = mouse_state.left_button_just_pressed;

    if (left_button_just_pressed)
    {
        std::cout << "Clicking in Interpolation mode..." << std::endl;
        interpolation_points.push_back(FramePoint(mouse_coord.x, mouse_coord.y, frame_index));
        if (interpolation_points.size() == 2)
        {
            std::cout << "Writing interpolation..." << std::endl;
            // Linear Interpolation Test
            int start_frame = interpolation_points[0].frame_index;
            int range = interpolation_points[1].frame_index - interpolation_points[0].frame_index;

            #define INCLUSIVE_INTERP 1

            #if !INCLUSIVE_INTERP
            // Only include middle frames:
            for (int i = 1; i < range; i++)
            {
                double t = static_cast<double>(i) / (range);
                Coordinate c = (1.0 - t) * interpolation_points[0].point + t * interpolation_points[1].point;
                brush.paint(frames[start_frame+i].pixels, c, FrameWidth, FrameHeight);
                frames[start_frame+i].texture.update(&frames[start_frame+i].pixels[0]);
            }
            #else
            // Include both ends inclusively as well:
            for (int i = 0; i <= range; i++)
            {
                double t = static_cast<double>(i) / (range);
                Coordinate c = (1.0 - t) * interpolation_points[0].point + t * interpolation_points[1].point;
                c = c - Coordinate(frame_offset.x, frame_offset.y);
                brush.paint(frames[start_frame+i].pixels, c, FrameWidth, FrameHeight);
                frames[start_frame+i].texture.update(&frames[start_frame+i].pixels[0]);
            }
            #endif

            interpolation_points.clear();
        }
    }
}
#endif // 0

/**
 * Assuming left mouse has already been clicked
 * // TODO: Just make this a member function of PaintController and use mouse_state
 */
void onGUI_selection(PaintModel& model, PaintView& view)
{
    auto& window       = view.getWindow();

    auto& full_menu    = view.m_gui.full_menu;

    auto& menu_icons       = view.m_gui.menu_icons;
    auto& menu_icon_colors = view.m_gui.menu_icon_colors;
    auto& brush_icons      = view.m_gui.brush_icons;
    auto& brush_icon_modes = view.m_gui.brush_icon_modes;

    auto& timeline_box   = view.m_gui.timeline_box;
    auto& frame_timeline = view.m_gui.frame_timeline;

    auto& frames         = model.raster_frames;
    auto& frame_index    = model.frame_index;
    auto& brush          = model.brush;

    sf::Vector2i pos = sf::Mouse::getPosition(window);
    if (full_menu.contains(sf::Mouse::getPosition(window)))
    {
        std::cout << "In menu box!" << std::endl;

        for (std::size_t i = 0; i < menu_icons.size(); i++)
        {
            if (menu_icons[i].getGlobalBounds().contains(sf::Vector2f(pos.x, pos.y)))
            {
                brush.color = menu_icon_colors[i];
                break;
            }
        }
        for (std::size_t i = 0; i < brush_icons.size(); i++)
        {
            if (brush_icons[i].getGlobalBounds().contains(sf::Vector2f(pos.x, pos.y)))
            {
                brush.mode = brush_icon_modes[i];
                break;
            }
        }
    }
    else if (timeline_box.contains(sf::Mouse::getPosition(window)))
    {
        for (std::size_t i = 0; i < frame_timeline.size(); i++)
        {
            if (frame_timeline[i].getGlobalBounds().contains(
                static_cast<sf::Vector2f>(sf::Mouse::getPosition(window))))
            {
                if (i < frames.size())
                {
                    frame_index = i;
                    std::cout << "index: " << i << std::endl;
                    break;
                }
                else
                    std::cout << "Frame doesn't exist yet..." << std::endl;
            }
        }
    }
    //else
    if (view.brush_menu_toggle_box.getGlobalBounds().contains(static_cast<sf::Vector2f>(pos)))
    {
        std::cout << "Toggling setVisible" << std::endl;
        view.brush_menu.setVisible(!view.brush_menu.isVisible());
    }
    // pulldown box logic handled elsewhere (for now)
}

inline
void onPulldownMenuClick(Menu& pulldown_menu, const sf::Vector2i& click_pos)
{
    for (InteractiveWidget* widget_ptr : pulldown_menu.m_widgets)
    {
        widget_ptr->onClick(click_pos);
    }
}

inline void fill_pixels(RawPixelArray& pixels, int Width, int Height, const Color& color)
{
    for (std::size_t i = 0; i < pixels.size(); i += 4)
    {
        pixels[i+0] = color.r;
        pixels[i+1] = color.g;
        pixels[i+2] = color.b;
        pixels[i+3] = color.a;
    }
}

/**
 * Handling Just clicked while in drawing area
 */
void PaintController::onDrawingAreaClick()
{
    auto& window = m_view.getWindow();
    auto& vector_frames = m_model.vector_frames;
    auto& frame_index = m_model.frame_index;
    auto& tool_mode = m_model.tool_mode;
    auto& view_mode = m_view.view_mode;

    std::cout << "In drawing area!" << std::endl;

    sf::Vector2f converted_pixel_coord = window.mapPixelToCoords(m_model.mouse_state.mouse_coord);

    if (view_mode == ViewMode::Raster)
    {
        if (tool_mode == ToolMode::PaintBucket)
        {
            std::size_t i = m_model.frame_index;
            fill_pixels(m_model.raster_frames[i].pixels,
                        m_model.frame_pixel_width, m_model.frame_pixel_width,
                        m_model.brush.color);
            m_model.raster_frames[i].refreshTexture();
        }
    }
    else if (view_mode == ViewMode::Vector)
    {
        if (tool_mode == ToolMode::Selection)
        {
            for (std::size_t i = 0; i < vector_frames[frame_index].circles.size(); i++)
            {
                auto center = vector_frames[frame_index].circles[i].getPosition();
                auto radius = vector_frames[frame_index].circles[i].getRadius();
                if (distance_squared(Coordinate(center.x, center.y), Coordinate(converted_pixel_coord.x, converted_pixel_coord.y))
                    < radius*radius)
                {
                    if (vector_frames[frame_index].circles[i].getFillColor() != sf::Color(255, 200, 20))
                    {
                        vector_frames[frame_index].circles[i].setFillColor(sf::Color(255, 200, 20));
                        break;
                    }

                }
            }
        }
        else if (tool_mode == ToolMode::Brush) // "Placement" mode?
        {
            vector_frames[frame_index].add_point(
              VectorRenderPoint(Coordinate(converted_pixel_coord.x, converted_pixel_coord.y), 20, VectorRenderStyle(Color::Red))
            );
        }
    }

    // Tool_Modes that share functionality between Vector and Raster
    if (tool_mode == ToolMode::Pan)
    {
        m_model.pan_state.origin = converted_pixel_coord;
    }
}


void PaintController::update_fromMouse()
{
    auto& view_mode    = m_view.view_mode;
    auto& window       = m_view.getWindow();
    auto& mouse_state  = m_model.mouse_state;

    auto  drawing_area_box = m_view.m_gui.drawing_area.getGlobalBounds();

    #if MOUSE_SYSTEM_IMMEDIATE
    mouse_state.old_mouse_coord = mouse_state.mouse_coord;
    mouse_state.mouse_coord     = sf::Mouse::getPosition(window);
    #endif

    // Note: It seems that no matter what order
    // update() and update_fromMouse are called, it shouldn't matter, because
    // mouse_state.left_button_just_pressed will only get set during update()'s onMousePress()
    if (mouse_state.left_button_just_pressed)
    {
        // Is there a way to simplify this logic...?
        // Perhaps check surrounding options menus's statuses
        // before checking exact mouse positions?
        m_view.m_main_menu.onMousePress(m_model, m_view, mouse_state.mouse_coord);

        // If clicked mouse is in drawing box:
        if (drawing_area_box.contains(static_cast<sf::Vector2f>(mouse_state.mouse_coord)))
        {
            if (m_view.brush_menu.isVisible())
            {
                if (m_view.brush_menu.box.getGlobalBounds().contains(static_cast<sf::Vector2f>(mouse_state.mouse_coord)))
                {
                    // Interacting with set of interactive widgets inside brush_menu
                    onPulldownMenuClick(m_view.brush_menu, mouse_state.mouse_coord);
                }
                else
                {
                    // Close the brush menu pulldown but don't allow drawing
                    m_view.brush_menu.setVisible(false);
                }
                // reset mouse state to prevent drawing:
                mouse_state.left_button_pressed = false;
            }
            else
            {
                // We're clicking inside the draw area and don't need to worry about GUI
                onDrawingAreaClick();
            }
        }
        else
        {
            if (m_view.brush_menu.isVisible())
            {
                if (m_view.brush_menu.box.getGlobalBounds().contains(static_cast<sf::Vector2f>(mouse_state.mouse_coord)))
                {
                    // Interacting with set of interactive widgets inside brush_menu
                    onPulldownMenuClick(m_view.brush_menu, mouse_state.mouse_coord);
                }
                else
                {
                    // Close the brush menu pulldown
                    m_view.brush_menu.setVisible(false);
                    onGUI_selection(m_model, m_view);
                    mouse_state.left_button_pressed = false; // see below
                }

            }
            else
            {
                // We're clicking in GUI zone
                onGUI_selection(m_model, m_view);
                // (temporary?) solution to not painting scene if
                // user clicked on GUI zone and *then* dragged.
                mouse_state.left_button_pressed = false;
            }
            // reset mouse_state to prevent drawing:
            mouse_state.left_button_pressed = false;
        }
    }
    else if (mouse_state.left_button_pressed)
    {
        // As long as left button is being pressed,
        // it will try to paint the screen.

        // TODO: It shouldn't paint the scene if
        // mouse button was pressed in the GUI zone,
        // and then dragged onto the drawing zone

        if (view_mode == ViewMode::Raster)
        {
            // Brush::Paint checks {x, y} bounds
            sf::Vector2f converted_old_pos  = window.mapPixelToCoords(mouse_state.old_mouse_coord);
            sf::Vector2f converted_curr_pos = window.mapPixelToCoords(mouse_state.mouse_coord);

            auto& tool_mode = m_model.tool_mode;

            if (tool_mode == ToolMode::Brush || tool_mode == ToolMode::Eraser)
            {
                onRasterDrawing(m_model, converted_old_pos, converted_curr_pos);
            }
            else if (tool_mode == ToolMode::RectangularSelection)
            {
                onRasterRectangularSelection(m_model, converted_curr_pos);
                onRasterRectangularSelection(m_model, m_view, converted_curr_pos);
            }
            else if (tool_mode == ToolMode::Move)
            {
                onMove(m_model);
                onMove(m_model, m_view);
            }
            else if (tool_mode == ToolMode::Pan)
            {
                onPan(m_model, converted_curr_pos);
                onPan(m_model, m_view, converted_curr_pos);
            }
            else if (tool_mode == ToolMode::Interpolation)
            {
                //onToolModeInterpolation()
            }
        }
    }

    mouse_state.left_button_just_pressed = false;
    // left_button_pressed is reset in onMouseRelease()
}

/**
 * Updates based on the action that needs to be performed
 */
void PaintController::update()
{
    auto& window = m_view.getWindow();
    if (to_mouse_press)
    {
        onMousePress(m_model, m_view.getWindow().mapPixelToCoords( sf::Mouse::getPosition(window) ));
        onMousePress(m_model, m_view, m_view.getWindow().mapPixelToCoords( sf::Mouse::getPosition(window)));
        to_mouse_press = false;
    }
    else if (to_mouse_release)
    {
        onMouseRelease(m_model);
        onMouseRelease(m_model, m_view);
        to_mouse_release = false;
    }
    if (to_plus)
    {
        onPlus(m_model, m_view);
        to_plus = false;
    }
    else if (to_minus)
    {
        onMinus(m_model, m_view);
        to_minus = false;
    }
    else if (to_right)
    {
        onRight(m_model, m_view);
        to_right = false;
    }
    else if (to_left)
    {
        onLeft(m_model, m_view);
        to_left = false;
    }
    else if (to_save)
    {
        onSave(m_model, m_view);
        to_save = false;
    }
    else if (to_save_alt)
    {
        onSaveAlt(m_model, m_view);
        to_save_alt = false;
    }
    else if (to_load)
    {
        onLoad(m_model, m_view);
        to_load = false;
    }
    else if (to_export)
    {
        onExport(m_model, m_view);
        to_export = false;
    }
    else if (to_delete)
    {
        /* delete_frames(&frames, frame_index);
        gui_elements.update_fromOnionChange();
        gui_elements.update_fromInsertion();
        gui_elements.update_fromDeletion(); */

        onDelete(m_model, m_view);
        to_delete = false;
    }
    else if (to_insert_pre)
    {
        onInsert(m_model, m_view, /*pre*/ true);
        to_insert_pre = false;
    }
    else if (to_insert_post)
    {
        onInsert(m_model, m_view, /*post*/ false);
        to_insert_post = false;
    }
    else if (to_copy)
    {
        onCopy(m_model, m_view);
        to_copy = false;
    }
    else if (to_paste)
    {
        onPaste(m_model, m_view);
        to_paste = false;
    }
    else if (to_expand)
    {
        onExpand(m_model, m_view);
        to_expand = false;
    }
    else if (to_undo)
    {
        onUndo(m_model, m_view);
        // If we want to be like Photoshop, add
        // && mouse is not pressed

        // TODO: Need to prevent undoing twice
        // and overwriting the pixels that are supposed to be saved for redoing
        // TODO: Also, implement this as a vector of undos for multiple undos/redos
        to_undo = false;
    }
    else if (to_redo)
    {
        onRedo(m_model, m_view);
        to_redo = false;
    }
    else if (to_duplicate)
    {
        onDuplicate(m_model, m_view);
        to_duplicate = false;
    }
    else if (to_zoom_in)
    {
        onZoomIn(m_model, m_view);
        to_zoom_in = false;
    }
    else if (to_zoom_out)
    {
        onZoomOut(m_model, m_view);
        to_zoom_out = false;
    }
    else if (to_run)
    {
        // "Running" in this case means playing the frames like an in-memory video.
        // At the preset frame rate.
        // The problem here is that this plays out through multiple frames,
        // where the frame needs to increment if the frame time has passed.
        // What is the best way to organize that?

        // Has to be some logical separation between actions that happen once
        // and continuous actions that span over multiple frames
        struct Process { // MultiframeProcess?
            bool running;
        };

        // Perhaps we should have a "onrunStart" function,
        // that then sets the model's ToolMode to Running?
        // or just onRun can do this.

        // Then in the other main event handler,
        // we check if tool_mode == ToolMode::RunAnimation
        // If it is, then we have to mutate the states in the
        // Note: This will sorta be like a singleton but it will still just be
        // an aggregrate of the model - RunAnimator? ScenePreview?
        // Also, while running the animation, Escape should exit,
        // Spacebar should pause.
        // Rename bool to_run as bool to_start_run

        onRun(m_model, m_view);
        to_run = false;
    }
}
