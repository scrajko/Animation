
#ifndef DEFAULT_FONT_HPP
#define DEFAULT_FONT_HPP

#define DEFAULT_FONT_INLINE inline

DEFAULT_FONT_INLINE
sf::Font make_defaultFont()
{
    sf::Font font;
    font.loadFromFile("cour.ttf");
    return font;
}

DEFAULT_FONT_INLINE
const sf::Font& defaultFont()
{
    const static sf::Font font = make_defaultFont();
    return font;
}

#endif // DEFAULT_FONT_HPP
