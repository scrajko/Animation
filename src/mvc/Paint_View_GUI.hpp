
#ifndef PAINT_VIEW_GUI_HPP
#define PAINT_VIEW_GUI_HPP

#include "Paint_Model.hpp"

#include <mvc/PulldownMenu.hpp>
#include <mvc/InteractiveWidgetMenu.hpp>

#include <SFML/Graphics.hpp>
#include <iostream>

// enum class FrameMode? ViewMode instead
enum class ViewMode {
    Raster,
    Vector
};

sf::VertexArray setupCenterGuidelines(const sf::IntRect& frame_box);

// "GUI_Config"? Screen vs. Window?

// TODO: Set Frame Dimensions as independent coordinates!
struct GUI_Layout {
    GUI_Layout(unsigned WindowWidth_, unsigned WindowHeight_,
               unsigned FrameWidth_, unsigned FrameHeight_);

    unsigned WindowWidth;
    unsigned WindowHeight;

    unsigned FrameWidth;
    unsigned FrameHeight;
};

struct GUI {
    GUI(const GUI_Layout& gui_layout);
    void drawMenu(sf::RenderWindow& window);
    void drawBehindFrame(sf::RenderWindow& window);
    void drawFront(sf::RenderWindow& window);

    // TODO: Get rid of useless member varibles,
    // simplify usage/dependencies
    //
    // The viewport must be proportioned correctly
    // with the size so that the frame is not distorted
    // at 100% zoom, each pixel should be exactly a pixel...

    //sf::IntRect drawing_area_box;
    sf::RectangleShape drawing_area;
    //sf::RectangleShape drawing_area;

    // Drawn behind the frame, to border all sides of it.
    // Needs to be big enough so that user can zoom out a large amount.
    sf::RectangleShape drawing_border;

    sf::Texture background_texture;
    sf::Sprite  background;

    sf::IntRect full_menu;

    std::vector<Color>              menu_icon_colors;
    std::vector<sf::RectangleShape> menu_icons;     //(menu_icon_colors.size());

    std::vector<BrushMode> brush_icon_modes;
    std::vector<sf::RectangleShape> brush_icons;    //(3);
    std::vector<sf::RectangleShape> frame_timeline; //(Num_Timeline_Frames);
    sf::IntRect timeline_box;

    sf::VertexArray guidelines;
};



/** Main View class for our painting/animation program thus far. */
// Idea: All functions that update things based from Model
// should have dedicated function to keep things organized.
class PaintView /*_SFML*/ {

  public:
    PaintView /*_SFML*/ (int Width, int Height, int FrameWidth, int FrameHeight, PaintModel& model);

    void draw();

    // Use GUI draws instead:
    void draw_GUI() const;
    void draw_Frame() const;

  public:
    sf::RenderWindow& getWindow()
    {
        return m_window;
    }

  private:
    void init_BrushViewing();
    void draw_current_frame();

  public: //private:
    sf::RenderWindow m_window;

    // Possible idea for organization:
    // Level 1: Menus, GUI elements user can't change
    // Level 2: GUI elements user can change
    // Level 3: GUI elements that depend on real-time user state (brush position)

    PaintModel&      m_model;

    ViewMode         view_mode;

    GUI_Layout       m_gui_layout;
    GUI              m_gui;
    //sf::IntRect      m_original_drawing_box;

    MainMenu         m_main_menu;
    //FrameView        m_frame_view_box;


    sf::CircleShape  brush_circle;

    sf::View         m_camera;

    int              m_zoom_level;
    // The texture sprites/objects drawn are all
    // orientated with respect to to the origin (0, 0),
    // but drawn through the view.
    // this view's viewport is the m_camera.zoom()

    Slider<unsigned char> brush_red_slider;
    Slider<unsigned char> brush_green_slider;
    Slider<unsigned char> brush_blue_slider;
    Slider<unsigned char> brush_alpha_slider;
    Slider<double>        brush_radius_slider;

    Menu brush_menu;

    sf::RectangleShape brush_menu_toggle_box;

    //sf::RectangleShape    brush_menu_box;
    //sf::RectangleShape    brush_menu_pulldown_box;
    //bool pulldown_menu_is_open;

    sf::RectangleShape selection_box;

    sf::RectangleShape current_color_box;
    sf::RectangleShape current_color_box_white;
};


// Make it so that Sliders always have to be
// dependent on a Menu.

// At start-up, all menus are created

#if 0
namespace gui {

    /* Bind the slider to the menu*/
    void bind(Slider& slider, Menu& menu)
    {
        //menu.
    }
}
#endif


/** Should be used once we start using OpenGL 4.0 texturing */
class PaintView_OpenGL {
  public:
    PaintView_OpenGL();
  private:
    sf::Window m_window;
};


#endif // PAINT_VIEW_GUI_HPP
