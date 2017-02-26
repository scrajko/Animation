
#ifndef INTERACTIVE_WIDGET_MENU_HPP
#define INTERACTIVE_WIDGET_MENU_HPP

#include <SFML/Graphics.hpp>

#include <iostream>
#include <vector>

inline int& slider_instances_count()
{
    static int n = 0;
    return n;
}


class InteractiveWidget {
  public:
    virtual void draw(sf::RenderWindow&);
    virtual void update();
    virtual void onClick(const sf::Vector2i& click_pos);
};

const int SliderLength = 255;
const int SliderHeight = 8;
template <typename T>
class Slider : public InteractiveWidget {
  public:
    Slider(T& source_value, T source_value_min, T source_value_max, sf::Vector2f position)
    : value(source_value),
      value_min(source_value_min),
      value_max(source_value_max)
    {
        value_box.setFillColor(sf::Color::Green);
        value_box.setPosition(position);
        update();

        back_box.setFillColor(sf::Color(50, 50, 50));
        back_box.setPosition(position);
        back_box.setSize(sf::Vector2f(SliderLength, SliderHeight));
    }
    void draw(sf::RenderWindow& window)
    {
        window.draw(back_box);
        window.draw(value_box);
    }
    void onClick(const sf::Vector2i& click_pos)
    {
        // Misleading name, since we actual check if clicked inside here

        if (back_box.getGlobalBounds().contains(static_cast<sf::Vector2f>(click_pos)))
        {
            std::cout << "Slider clicked" << std::endl;
            double min_value = value_min;
            double max_value = value_max;
            double min_pos = back_box.getGlobalBounds().left;

            double t = (click_pos.x - min_pos) / (back_box.getGlobalBounds().width);
            value = (1 - t) * min_value + t * max_value;
            update();
        }
    }

    void update()
    {
        double fill_length = SliderLength * static_cast<double>(value - value_min) / (value_max - value_min);
        value_box.setSize(sf::Vector2f(fill_length, SliderHeight));
    }

  public:
    T& value;
    T value_min;
    T value_max;

    sf::RectangleShape value_box;
    sf::RectangleShape back_box;
};


class Menu {

  public:
    Menu()
    : box(),
      m_widgets(),
      m_visible(true)
    {
        box.setSize(sf::Vector2f(500, 300));
        box.setPosition(700, 10);
        box.setFillColor(sf::Color(255, 0, 255));
    }
    void add_widget_ptr(InteractiveWidget& widget)
    {
        m_widgets.push_back(&widget);
    }

    void draw(sf::RenderWindow& window)
    {
        if (isVisible())
        {
            window.draw(box);
            for (InteractiveWidget* widget_ptr : m_widgets)
            {
                widget_ptr->update();
                widget_ptr->draw(window);
            }
        }
    }

    bool isVisible()
    {
        return m_visible;
    }

    void setVisible(bool visiblity_toggle = true)
    {
        m_visible = visiblity_toggle;
    }

  public:
    sf::RectangleShape box;

  public:
    std::vector<InteractiveWidget*> m_widgets;

  private:
    bool m_visible;
};


/*
 * A Slider is a GUI element that is connected to
 * a portion of the model.
 *
 * value_box's size is adjusted base on the reference value.
 */ /*
const int SliderLength = 100;
const int SliderHeight = 4;
template <typename T>
class Slider {
  public:

    Slider(T& source_value, T source_value_min, T source_value_max)
    : value(source_value),
      value_min(source_value_min),
      value_max(source_value_max)
    {
        value_box.setFillColor(sf::Color::Green);
        value_box.setPosition(100, -20 + slider_instances_count() * 15);
        update();

        back_box.setFillColor(sf::Color(50, 50, 50));
        back_box.setPosition(100,  -20 + slider_instances_count() * 15);
        back_box.setSize(sf::Vector2f(SliderLength, SliderHeight));

        slider_instances_count()++;
    }

    void draw(sf::RenderWindow& window)
    {
        window.draw(back_box);
        window.draw(value_box);
    }

    void update()
    {
        double fill_length = SliderLength * static_cast<double>(value - value_min) / (value_max - value_min);
        value_box.setSize(sf::Vector2f(fill_length, SliderHeight));
    }


  //private:
  public:
    T& value;
    T value_min;
    T value_max;

    sf::RectangleShape value_box;
    sf::RectangleShape back_box;

    bool isCircleBeingHeld;

};
*/

/*
void onClick_Frame_Raster(RenderFrame& frame)
{}

void onClick_Frame_Vector(VectorFrame& frame)
{}
*/


#endif // INTERACTIVE_WIDGET_MENU_HPP
