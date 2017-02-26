
#ifndef PULLDOWN_MENU_HPP
#define PULLDOWN_MENU_HPP

#include <SFML/Graphics.hpp>

#include <iostream>
#include <string>
#include <vector>

/*
 * TODO: Change into tree structure for
 * more customizability
 *
 * TODO: Still need to connect this so that something is calling the onClick commands
 */

class PaintModel;
class PaintView;

class MainMenu {
  public:
    MainMenu();

    void load_fromFile(const std::string& filename);
    void load_manual();

    void onMousePress(PaintModel& model, PaintView& view, const sf::Vector2i& mouse_coord);
    void onMouseRelease(PaintModel& model, PaintView& view, const sf::Vector2i& mouse_coord);

    void draw(sf::RenderWindow& window);

  public:
    static const int Offset_Left =  0;
    static const int Offset_Top  =  0;
    static const int Box_Width   = 50; //20;
    static const int Box_Height  = 14; //10;

    std::vector<sf::RectangleShape>              main_menu_boxes;
    std::vector<std::vector<sf::RectangleShape>> pulldown_menu_boxes;

    std::vector<sf::Text>                        main_menu_texts;
    std::vector<std::vector<sf::Text>>           pulldown_menu_texts;

    //std::vector<std::vector<PulldownFunction>>   pulldown_menu_functions;
    std::vector<std::vector<void(*)(PaintModel&, PaintView&)>>   pulldown_menu_functions;

  private:
    bool        pulldown_menu_open;
    std::size_t curr_selected_box_index;

};


#endif // PULLDOWN_MENU_HPP
