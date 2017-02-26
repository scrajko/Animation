
#include "Paint_View_GUI.hpp"

// This file contains implementation code for
// the Paint GUI and the Paint View

// TODO: Look at the bottom of this file,
// reorganize that Circle/Brush stuff

sf::VertexArray setupCenterGuidelines(const sf::FloatRect& frame_box)
{
    sf::VertexArray guidelines(sf::Lines, 4);
    guidelines[0].position = sf::Vector2f(frame_box.left + frame_box.width/2.0,
                                          frame_box.top  + 0);
    guidelines[1].position = sf::Vector2f(frame_box.left + frame_box.width/2.0,
                                          frame_box.top  + frame_box.height);
    guidelines[2].position = sf::Vector2f(frame_box.left,
                                          frame_box.top  + frame_box.height/2.0);
    guidelines[3].position = sf::Vector2f(frame_box.left + frame_box.width,
                                          frame_box.top  + frame_box.height/2.0);
    return guidelines;
}

GUI_Layout::GUI_Layout(unsigned WindowWidth_, unsigned WindowHeight_,
                       unsigned FrameWidth_, unsigned FrameHeight_)
: WindowWidth    (WindowWidth_),
  WindowHeight   (WindowHeight_),
  FrameWidth     (FrameWidth_),
  FrameHeight    (FrameHeight_)
{
    #if 0
    const unsigned TopHeight = (WindowHeight - FrameHeight) / 2;
    const unsigned LeftWidth = (WindowWidth  - FrameWidth)  / 2;

    frame_box    = sf::IntRect(LeftWidth, TopHeight, FrameWidth, FrameHeight);
    frame_offset = sf::Vector2i(LeftWidth, TopHeight);
    #endif
}

///////////////////
///  Paint GUI  ///
///////////////////


/*
/// December 26, 2015 update, in attempt to make more flexible
GUI::GUI(int WindowWidth, int WindowHeight)
: stuff()
{
    // Settuing up border, these stay constant:
    const int MenuTopHeight    = 50;
    const int MenuBottomHeight = 20;
    const int MenuLeftWidth    = 50;
    const int MenuRightWidth   = 100;

    // Set up box that user can draw in
    // That surrounds the pixel box.
    drawing_area = sf::RectangleShape( sf::Vector2f(
        WindowWidth - MenuLeftWidth - MenuRightWidth,
        WindowHeight - MenuTopHeight - MenuBottomHeight
    );
    drawing_area.setPosition(sf::Vector2f(MenuLeftWidth, MenuTopHeight));
    drawing_area.setFillColor(sf::Color(255, 0, 255));

    // The sf::View is what allows the pixel area to be drawn within its own box
    // The sf::View's dimensions should be the same as the drawing area, never going outside it

}
/////////////////////////////////////////
*/

GUI::GUI(const GUI_Layout& gui_layout)
: //drawing_border_box(),
  drawing_area       (),
  drawing_border     (),
  background_texture (),
  background         (),
  full_menu          (),  // Reassigned later
  menu_icon_colors   (),  // Reassigned later
  menu_icons         (),  // Resized later

  brush_icon_modes   (),  // Reassigned later
  brush_icons        (),  // Resized later
  frame_timeline     (),  // Resized later
  timeline_box       (),  // Reassigned later
  guidelines         (setupCenterGuidelines(drawing_area.getGlobalBounds()))
{
    auto& WindowWidth = gui_layout.WindowWidth;
    auto& WindowHeight = gui_layout.WindowHeight;

    // Settuing up border, these stay constant:
    const int MenuTopHeight    = 100;
    const int MenuBottomHeight = 30;
    const int MenuLeftWidth    = 100;
    const int MenuRightWidth   = 150;

    // Set up box that user can draw in
    // That surrounds the pixel box.
    drawing_area = sf::RectangleShape( sf::Vector2f(
        WindowWidth - MenuLeftWidth - MenuRightWidth,
        WindowHeight - MenuTopHeight - MenuBottomHeight
    ));
    drawing_area.setPosition(sf::Vector2f(MenuLeftWidth, MenuTopHeight));
    drawing_area.setFillColor(sf::Color(255, 0, 255));

    // The sf::View is what allows the pixel area to be drawn within its own box
    // The sf::View's dimensions should be the same as the drawing area, never going outside it

    const unsigned TopHeight = MenuTopHeight; //(gui_layout.WindowHeight - gui_layout.FrameHeight) / 2;
    const unsigned LeftWidth = MenuLeftWidth; //(gui_layout.WindowWidth  - gui_layout.FrameWidth)  / 2;

    int padding       = 5;
    int MenuWidth     = LeftWidth;
    int SubMenuHeight = gui_layout.WindowHeight/2;
    int icon_width    = MenuWidth / 4;
    int icon_height   = MenuWidth / 4;

    /*
    // drawing_are dimensions should match drawing_area_box
    drawing_area.setSize(sf::Vector2f(drawing_area_box.width, drawing_area_box.height));
    drawing_area.setPosition(drawing_area_box.left, drawing_area_box.top);
    drawing_area.setFillColor(sf::Color(60, 60, 60));*/

    //const std::string background_filename = "gui/PS_transparent.png";
    const std::string background_filename = "gui/PS_transparent_xlarge.png";
    if (!background_texture.loadFromFile(background_filename))
    {
        std::cout << "error loading texture " << background_filename << std::endl;
    }

    // Background is behind the Frame Area, for looks.
    // It should be the same size and place as the Frame.
    // We're putting it at (0, 0) like the Frame for convenience,
    // the sf::View handles the offsetting.

    background = sf::Sprite(background_texture,
                   sf::IntRect(0, 0, gui_layout.FrameWidth, gui_layout.FrameHeight));
    background.setPosition(0, 0);

    full_menu = sf::IntRect(0, 0, MenuWidth + 2*padding, gui_layout.WindowHeight);
    sf::IntRect color_menu(padding, padding, MenuWidth, SubMenuHeight - 2*padding);
    sf::IntRect brush_menu(padding, 2*padding + SubMenuHeight, MenuWidth, SubMenuHeight - 2*padding);

    menu_icon_colors = std::vector<Color>
      {Color::Black, Color::White,
       Color::Red,   Color::Blue,
       Color::Green, Color(255, 127, 0)};
    //std::vector<sf::RectangleShape> menu_icons(menu_icon_colors.size());
    menu_icons.resize(menu_icon_colors.size());
    for (std::size_t i = 0; i < menu_icons.size(); i++)
    {
        auto& icon = menu_icons[i];
        icon.setFillColor(sf::Color(menu_icon_colors[i].r,
                                    menu_icon_colors[i].g,
                                    menu_icon_colors[i].b));
        icon.setSize(sf::Vector2f(icon_width, icon_height));
        icon.setPosition(padding + (i%2)*icon_width, padding + (i/2)*icon_height);
    }

    brush_icon_modes = std::vector<BrushMode> {
      BrushMode::AlphaCircle,
      BrushMode::Rectangle,
      BrushMode::Eraser
    };

    //std::vector<sf::RectangleShape> brush_icons(3);
    brush_icons.resize(3);
    brush_icons[0].setFillColor(sf::Color(128, 128, 128));
    brush_icons[1].setFillColor(sf::Color(200, 200, 200));
    brush_icons[2].setFillColor(sf::Color( 50,  50,  50));
    for (std::size_t i = 0; i < brush_icons.size(); i++)
    {
        auto& icon = brush_icons[i];
        icon.setSize(sf::Vector2f(icon_width, icon_height));
        icon.setPosition(padding + (i%2)*icon_width, padding + padding + SubMenuHeight + (i/2)*icon_height );
    }

    const std::size_t Timeline_Width            = gui_layout.FrameWidth;
    const std::size_t Timeline_Height           = 15;
    const std::size_t Num_Timeline_Frames       = 20;
    const std::size_t Individual_Timeline_Width = Timeline_Width / Num_Timeline_Frames;

    //std::vector<sf::RectangleShape> frame_timeline(Num_Timeline_Frames);
    frame_timeline.resize(Num_Timeline_Frames);

    timeline_box = sf::IntRect(LeftWidth, TopHeight + gui_layout.FrameHeight + 10,
                               Timeline_Width, Timeline_Height);
    sf::Vector2i top_timeline_corner(LeftWidth, TopHeight + gui_layout.FrameHeight + 10);
    for (std::size_t i = 0; i < Num_Timeline_Frames; i++)
    {
        frame_timeline[i] = sf::RectangleShape(
          sf::Vector2f(Individual_Timeline_Width - 1, Timeline_Height));
        frame_timeline[i].setPosition(
          sf::Vector2f(top_timeline_corner.x + i*Individual_Timeline_Width, top_timeline_corner.y));
        frame_timeline[i].setFillColor(sf::Color::Green);
    }
}

/**
 * TODO: Set up View/GUI drawing functions
 * to first draw things under frames,
 * then draw the frame,
 * then draw things over the frame.
 * Total of 3 parts/functions.
 */
void GUI::drawMenu(sf::RenderWindow& window)
{
    for (auto& icon : menu_icons)
        window.draw(icon);
    for (auto& icon : brush_icons)
        window.draw(icon);
    for (auto& box : frame_timeline)
        window.draw(box);


}
void GUI::drawBehindFrame(sf::RenderWindow& window)
{
    // drawing_border is below "transparent" background
    // which is below the actual pixels,
    // which are below the guidelines.
    // sf::View should be set externally before this.


    window.draw(drawing_border);
    window.draw(background);

}
void GUI::drawFront(sf::RenderWindow& window)
{
    //window.draw(guidelines);
}


////////////////////
///  Paint View  ///
////////////////////

// TODO: Route all the Frame Dimension requirements to
// a more singular point to allow more flexibility

PaintView::PaintView(int WindowWidth, int WindowHeight,
                     int FrameWidth, int FrameHeight,
                     PaintModel& model)
: m_window(sf::VideoMode(WindowWidth, WindowHeight), "Animation Paint 2.1", sf::Style::Close),
  m_model(model),
  view_mode(ViewMode::Raster),
  m_gui_layout(WindowWidth, WindowHeight, FrameWidth, FrameHeight),
  m_gui(m_gui_layout),
  /*m_original_drawing_box(sf::IntRect(m_gui_layout.frame_offset.x,
                                     m_gui_layout.frame_offset.y,
                                     m_gui_layout.FrameWidth,
                                     m_gui_layout.FrameHeight)),*/

  m_main_menu(),

  brush_red_slider   (m_model.brush.color.r,     0, 255, sf::Vector2f(700, 50*1)),
  brush_green_slider (m_model.brush.color.g,     0, 255, sf::Vector2f(700, 50*2)),
  brush_blue_slider  (m_model.brush.color.b,     0, 255, sf::Vector2f(700, 50*3)),
  brush_alpha_slider (m_model.brush.color.a,     0, 255, sf::Vector2f(700, 50*4)),
  brush_radius_slider(m_model.brush.max_radius,  0, 200, sf::Vector2f(700, 50*5)),
  brush_menu(),

  brush_menu_toggle_box()

  //brush_radius_slider (m_model.brush.radius, 0, 200),
  //brush_opacity_slider(m_model.brush.color.a, 0, 255),

{
    init_BrushViewing();

    /* New idea: Draw the frame box over just a really big
       background square. 100% shows some of the background on the edges.
       Then if you zoom out, (have a max zoom out set up)
       it will just show a small frame with the big background.
       and if you zoom in, it will show just the frame box
       We can worry about drawing an independently-sized transparency background later
    */

    // THE +0.5 HELPS IMMENSLY TO FIX THE PIXEL-PERFECT ROUND OFF ERRORS
    // Other problem still is that when you zoom in and then out,
    // minor rounding errors show up because the zoom in zoom out isn't 100% reversible.
    // We need to have a "zoom factor" int variable that we factor the setSize() function by to solve this.
    //
    // Also need to make sure movement is properly aligned too!
    //
    auto drawing_area_box = m_gui.drawing_area.getGlobalBounds();

    m_camera.setCenter(sf::Vector2f(m_gui_layout.FrameWidth/2.0f + 0.5,
                                    m_gui_layout.FrameHeight/2.0f + 0.5));
    m_camera.setSize(drawing_area_box.width,
                     drawing_area_box.height);

    double left_draw   = static_cast<double>(drawing_area_box.left)   / m_gui_layout.WindowWidth;
    double top_draw    = static_cast<double>(drawing_area_box.top)    / m_gui_layout.WindowHeight;
    double width_draw  = static_cast<double>(drawing_area_box.width)  / m_gui_layout.WindowWidth;
    double height_draw = static_cast<double>(drawing_area_box.height) / m_gui_layout.WindowHeight;

    // This viewport should be OK and constant, this is where we want the extent of the frame
    // to be drawn, and the background to be drawn if the frame is too big.
    m_camera.setViewport(sf::FloatRect(left_draw, top_draw, width_draw, height_draw));

    m_main_menu.load_manual();

    brush_menu.add_widget_ptr(brush_red_slider);
    brush_menu.add_widget_ptr(brush_green_slider);
    brush_menu.add_widget_ptr(brush_blue_slider);
    brush_menu.add_widget_ptr(brush_alpha_slider);
    brush_menu.add_widget_ptr(brush_radius_slider);

    brush_menu_toggle_box.setPosition(700, 10);
    brush_menu_toggle_box.setSize(sf::Vector2f(20, 20));

    //pulldown_menu_is_open = false;

    selection_box.setFillColor(sf::Color::Transparent);
    selection_box.setOutlineThickness(1);
    selection_box.setOutlineColor(sf::Color::Red);

    current_color_box_white.setFillColor(sf::Color::White);
    current_color_box_white.setSize(sf::Vector2f(40, 40));
    current_color_box_white.setPosition(sf::Vector2f(10, 240));

    current_color_box.setFillColor(sf::Color(m_model.brush.color.r, m_model.brush.color.g,
                                             m_model.brush.color.b, m_model.brush.color.a));
    current_color_box.setSize(sf::Vector2f(40, 40));
    current_color_box.setPosition(sf::Vector2f(10, 240));
}

void PaintView::draw_current_frame()
{
    auto& frames        = m_model.raster_frames;
    auto& vector_frames = m_model.vector_frames;
    auto& frame_index   = m_model.frame_index;

    if (view_mode == ViewMode::Raster)
    {
        ////////////////////////////////////////////////////
        /// Draw an onion-skin frame in reduced opacity  ///
        ////////////////////////////////////////////////////
         // TODO: More customization
         // TODO: For example, also rendering a "reference frame"
         // that's always there no matter what frame you're on,
         // that acts as a guide so that the animation doesn't become
         // distorted over time.
        if (frame_index > 0)
        {
            frames[frame_index-1].sprite.setColor(sf::Color(255, 255, 255, 66));
            m_window.draw(frames[frame_index-1].sprite);
            frames[frame_index-1].sprite.setColor(sf::Color::White);
        }

        m_window.draw(frames[frame_index].sprite);
        m_window.draw(m_model.premerged_brush_layer_sprite);
    }
    else if (view_mode == ViewMode::Vector)
    {
        for (auto& circle : vector_frames[frame_index].circles)
        {
            // TODO: offsets for the circle
            // Gotta implement a "View"
            // Offsets and zoom!
            //
            // Also I set the circle to be Red but it's still white.
            //
            //circle.move(m_gui_layout.frame_offset.x,
            //            m_gui_layout.frame_offset.y);
            m_window.draw(circle);
            //circle.move(-m_gui_layout.frame_offset.x,
            //            -m_gui_layout.frame_offset.y);

            // In OpenGL:
            // TODO: Make test program with uniform shader stuff
            // to test out movement and zoom
            //glUniformSomething

        }
    }
}


    // The "Photoshop-style transparency" frame background square size should not zoom in
    // But the grey area around the frame should disappear if we are zoomed in far enough

    ///
    /// 1. Set sf::View to default window view
    /// 2. Draw background GUI elements first.
    ///    - Including background of "drawing area"
    ///
    /// 3. Set sf::View to the one for the Frame area.
    /// 4. Draw frame
    /// 5. Draw over-frame things (like selection box) which depend on frame zoom
    ///
    /// 6. Draw non-frame foreground GUI elements (if any)
    ///
void PaintView::draw()
{
    m_window.clear(sf::Color(70, 70, 90));

    m_window.setView(m_window.getDefaultView());
    m_gui.drawMenu(m_window);
    m_main_menu.draw(m_window);

    m_window.draw(m_gui.drawing_area);

    m_window.setView(m_camera);
    m_gui.drawBehindFrame(m_window);

    draw_current_frame();

    if (m_model.rectangular_selection_state.active || m_model.tool_mode == ToolMode::RectangularSelection)
    {
        m_window.draw(selection_box);
        m_window.draw(m_model.rectangular_selection_state.floating_selection.sprite);
    }
    else
    // Eh, might not be needed, but I moved the circle's position at the last possible moment.
    // Ew... should have... ToolMode::Interpolation::ShowCursor virtual call?
    if (m_model.tool_mode < ToolMode::BrushCircleCutoff)
    {
        brush_circle.setPosition(static_cast<sf::Vector2f>(sf::Mouse::getPosition(m_window)));
        m_window.draw(brush_circle);
    }
    else
    { /* Need to draw something custom...*/ }

    m_window.setView(m_window.getDefaultView());

    m_gui.drawFront(m_window);

    // TODO: This is not the best place to put this

    current_color_box.setFillColor(sf::Color(m_model.brush.color.r, m_model.brush.color.g,
                                             m_model.brush.color.b, m_model.brush.color.a));
    m_window.draw(current_color_box_white);
    m_window.draw(current_color_box);

    if (brush_menu.isVisible())
    {
        brush_menu.draw(m_window);
    }
    m_window.draw(brush_menu_toggle_box);

    // Changed it back to m_camera view
    // because this apparently affects external functionality,
    // most likely mapping stuff
    // TODO: Restructure
    //
    m_window.setView(m_camera);


    //test
    //m_window.setView(m_window.getDefaultView());
    //m_window.draw(m_model.raster_frames[0].sprite);

    // Seems to get a bit distorted in both directions,
    // make the alternative red blue red blue pixel background bigger to compare.

    // Also open up the alternating pattern in Photoshop to see how it decides to render it.
    // MAKE SURE THE VIEWPORT IS AN INTEGER BOUNDS!
    // As in, the code where you have (box length)/screen_width),
    // Round that to the nearest pixel or something.
    m_window.display();
}

void PaintView::draw_GUI() const
{

}

void PaintView::draw_Frame() const
{

}

inline void update_brush_circle(sf::CircleShape& brush_circle, const Brush& brush)
{
    brush_circle.setRadius(brush.radius);
    brush_circle.setOrigin(brush.radius, brush.radius);
    //TODO: Only change when color changes...
    brush_circle.setOutlineColor(sf::Color(brush.color.r, brush.color.g,
                                           brush.color.b, brush.color.a));
}

void PaintView::init_BrushViewing()
{
    // Initialize/update this based on model:
    sf::CircleShape brush_circle(10);
    brush_circle.setFillColor(sf::Color::Transparent);
    brush_circle.setOutlineThickness(1);
    update_brush_circle(brush_circle, Brush(8)); // default brush
}
