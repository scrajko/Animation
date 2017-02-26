
#include "PulldownMenu.hpp"

#include <fstream>
#include <iostream>

#include <mvc/MenuFunctions.hpp>
#include <mvc/DefaultFont.hpp>

MainMenu::MainMenu()
: pulldown_menu_open(false),
  curr_selected_box_index(0)
{}

/**
 * External tests should check if any part of the menu bar was clicked.
 */
void MainMenu::onMousePress(PaintModel& model, PaintView& view, const sf::Vector2i& mouse_coord)
{
    // Iterate to see if selecting one of the horizontal boxes
    for (std::size_t i = 0; i < main_menu_boxes.size(); i++)
    {
        if (main_menu_boxes[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(mouse_coord)))
        {
            if (curr_selected_box_index == i)
            {
                // Make pulldown open if it was closed and vice versa
                pulldown_menu_open = !pulldown_menu_open;
                curr_selected_box_index = i;
            }
            else
            {
                // Make pulldown visible
                pulldown_menu_open = true;
                curr_selected_box_index = i;
            }
            return;
        }
    }

    if (pulldown_menu_open)
    {
        for (std::size_t i = 0; i < pulldown_menu_boxes[curr_selected_box_index].size(); i++)
        {
            if (pulldown_menu_boxes[curr_selected_box_index][i].getGlobalBounds().contains(static_cast<sf::Vector2f>(mouse_coord)))
            {
                // if (not grayed out)

                // TODO: It should actually do the action on mouse *release* and not mouse press.
                std::cout << "Performing pulldown function..." << std::endl;
                pulldown_menu_functions[curr_selected_box_index][i](model, view);
                pulldown_menu_open = false;
            }
        }
    }
}

void MainMenu::onMouseRelease(PaintModel& model, PaintView& view, const sf::Vector2i& mouse_coord)
{

}

void MainMenu::load_fromFile(const std::string& filename)
{
    std::ifstream f(filename); //  app_data/menu.txt
    if (!f)
    {
        std::cout << "PulldownMenu::load_fromFile(): Unable to open " << filename << std::endl;
        return;
    }

    // Need to map the functionality of each button

}


void MainMenu::load_manual()
{
    main_menu_boxes.resize(4);
    main_menu_texts.resize(4);

    pulldown_menu_boxes.resize(4);
    pulldown_menu_texts.resize(4);

    pulldown_menu_functions.resize(4);

    for (std::size_t i = 0; i < main_menu_boxes.size(); i++)
    {
        main_menu_boxes[i].setSize(sf::Vector2f(Box_Width, Box_Height));
        main_menu_boxes[i].setPosition(sf::Vector2f(Offset_Left + i*Box_Width, Offset_Top));
        main_menu_boxes[i].setFillColor(sf::Color(200, 200, 200));
        main_menu_boxes[i].setOutlineColor(sf::Color(100, 100, 00));
        main_menu_boxes[i].setOutlineThickness(2);

        main_menu_texts[i].setPosition(sf::Vector2f(Offset_Left + i*Box_Width, Offset_Top));
        main_menu_texts[i].setColor(sf::Color(0, 0, 128));
        main_menu_texts[i].setFont(defaultFont());
        main_menu_texts[i].setCharacterSize(12);
    }

    // Main Texts:
    main_menu_texts[0].setString("File");
    main_menu_texts[1].setString("Edit");
    main_menu_texts[2].setString("Tools");
    main_menu_texts[3].setString("Effects");

    // File:
    pulldown_menu_boxes[0].resize(4);
    pulldown_menu_texts[0].resize(4);

    pulldown_menu_functions[0].resize(4);
    pulldown_menu_functions[0][0] = menu_FileNew;   /* New     */;
    pulldown_menu_functions[0][1] = menu_FileSave;  /* Save    */;
    pulldown_menu_functions[0][2] = menu_FileSaveAs;/* Save As */;
    pulldown_menu_functions[0][3] = menu_FileExit;  /* Exit    */;


    pulldown_menu_texts[0][0].setString("New");
    pulldown_menu_texts[0][1].setString("Save");
    pulldown_menu_texts[0][2].setString("Save As");
    pulldown_menu_texts[0][3].setString("Exit");

    // Edit:
    pulldown_menu_boxes[1].resize(2);
    pulldown_menu_texts[1].resize(2);

    pulldown_menu_functions[1].resize(2);
    pulldown_menu_functions[1][0] = menu_EditUndo; /* Undo    */;
    pulldown_menu_functions[1][1] = menu_EditRedo; /* Redo    */;

    pulldown_menu_texts[1][0].setString("Undo");
    pulldown_menu_texts[1][1].setString("Redo");

    // Tools:
    pulldown_menu_boxes[2].resize(5);
    pulldown_menu_texts[2].resize(5);

    pulldown_menu_functions[2].resize(5);
    pulldown_menu_functions[2][0] = menu_ToolsBrush;
    pulldown_menu_functions[2][1] = menu_ToolsEyedropper;
    pulldown_menu_functions[2][2] = menu_ToolsBucket;
    pulldown_menu_functions[2][3] = menu_ToolsMove;
    pulldown_menu_functions[2][4] = menu_ToolsSquareSelection;

    pulldown_menu_texts[2][0].setString("Brush");
    pulldown_menu_texts[2][1].setString("Eyedropper");
    pulldown_menu_texts[2][2].setString("Bucket");
    pulldown_menu_texts[2][3].setString("Move");
    pulldown_menu_texts[2][4].setString("Square Selection");

    // Effects:
    pulldown_menu_boxes[3].resize(2);
    pulldown_menu_texts[3].resize(2);

    pulldown_menu_functions[3].resize(2);
    pulldown_menu_functions[3][0] = menu_EffectsCrystallization;
    pulldown_menu_functions[3][1] = menu_EffectsBlur;

    pulldown_menu_texts[3][0].setString("Crystallization");
    pulldown_menu_texts[3][1].setString("Blur");

    // Setup pulldown menu boxes:
    for (std::size_t i = 0; i < pulldown_menu_boxes.size(); i++)
    for (std::size_t j = 0; j < pulldown_menu_boxes[i].size(); j++)
    {
        pulldown_menu_boxes[i][j].setSize(sf::Vector2f(Box_Width, Box_Height));
        pulldown_menu_boxes[i][j].setPosition(sf::Vector2f(Offset_Left + i*Box_Width, Offset_Top + Box_Height + j*Box_Height));
        pulldown_menu_boxes[i][j].setFillColor(sf::Color(200, 200, 200));
        pulldown_menu_boxes[i][j].setOutlineColor(sf::Color(100, 100, 00));
        pulldown_menu_boxes[i][j].setOutlineThickness(1);

        pulldown_menu_texts[i][j].setPosition(sf::Vector2f(Offset_Left + i*Box_Width, Offset_Top + Box_Height + j*Box_Height));
        pulldown_menu_texts[i][j].setFont(defaultFont());
        pulldown_menu_texts[i][j].setColor(sf::Color(0, 0, 255));
        pulldown_menu_texts[i][j].setCharacterSize(12);
    }

    std::cout << "Manually loaded the pulldown menu" << std::endl;
}

void MainMenu::draw(sf::RenderWindow& window)
{
    for (auto& box : main_menu_boxes)
    {
        window.draw(box);
    }
    for (auto& text : main_menu_texts)
    {
        window.draw(text);
    }

    if (pulldown_menu_open)
    {
        for (auto& box : pulldown_menu_boxes[curr_selected_box_index])
        {
            window.draw(box);
        }
        for (auto& text : pulldown_menu_texts[curr_selected_box_index])
        {
            window.draw(text);
        }
    }
}
