
#ifndef INPUT_BOX_HPP
#define INPUT_BOX_HPP

#include <SFML/Graphics.hpp>

#include <string>
#include <sstream>
#include <iostream>

#include <mvc/DefaultFont.hpp>

template <typename T>
class InputBox {
  public:
    InputBox(const std::string& input_statement, T& answer_space);

    void run();
    void render();

  private:
    sf::RenderWindow window;
    T& answer;
    sf::Text input_question_text;
    sf::String answer_string;
    sf::Text answer_text;
};

template <typename T>
InputBox<T>::InputBox(const std::string& input_statement, T& answer_space)
: window(sf::VideoMode(300, 100), "Requesting Input"),
  answer(answer_space),
  input_question_text(input_statement, defaultFont(), 20),
  answer_string(),
  answer_text("", defaultFont(), 20)
{
    double height = input_question_text.getLocalBounds().height;
    height = height * 1.4;
    answer_text.move(sf::Vector2f(0.0, height));
}

template <typename T>
void InputBox<T>::run()
{
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::TextEntered)
            {
                if (event.text.unicode < 128)
                {
                    if (event.text.unicode == 13) // Enter key
                    {
                        std::cout << "Processing answer..." << std::endl;

                        std::string answer_cpp_string = answer_string.toAnsiString();
                        std::stringstream ss(answer_cpp_string);

                        // >> is space delimited
                        // doesn't handle spaced strings!
                        if ((ss >> answer))
                        {
                            window.close();
                        }
                        else
                        {
                            // TODO: Could have more dynamic failure clauses,
                            // ex: Number be >= 0 and <= 255
                            // pass an std::vector of function pointers
                            std::cout << "Error converting input. Keep try." << std::endl;
                        }
                    }
                    else if (event.text.unicode == 8) // Backspace key
                    {
                        std::string answer_cpp_string = answer_string.toAnsiString();
                        if (!answer_cpp_string.empty())
                            answer_cpp_string.pop_back();
                        answer_string = sf::String(answer_cpp_string);
                    }
                    else
                    {
                        answer_string += static_cast<char>(event.text.unicode);
                    }
                }
            }
        }
        answer_text.setString(answer_string);
        render();
    }
}

template <typename T>
void InputBox<T>::render()
{
    window.clear();
    window.draw(input_question_text);
    window.draw(answer_text);
    window.display();
}

#endif // INPUT_BOXX_HPP
