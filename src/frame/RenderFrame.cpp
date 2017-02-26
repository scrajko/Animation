
#include <frame/RenderFrame.hpp>

#include <graphics/Color.hpp>

void RenderFrame::init_texture(const sf::IntRect& box, bool clear_pixels /* = true */)
{
    // Texture is not actually copied, but made anew!
    texture.create(box.width, box.height);
    sprite.setTexture(texture);
    sprite.setPosition(box.left, box.top);

    if (clear_pixels)
    {
        Color background = Color(0, 0, 0, 0);
        for (std::size_t i = 0; i < box.width * box.height * 4; i+=4)
        {
            pixels[i]   = background.r;
            pixels[i+1] = background.g;
            pixels[i+2] = background.b;
            pixels[i+3] = background.a;
        }
    }
    texture.update(&pixels[0]);
}

/**
 * Copy Constructor
 */
RenderFrame::RenderFrame(const RenderFrame& other)
: pixels(other.pixels),
  sprite(other.sprite)
{
    sf::Vector2u s = other.texture.getSize();
    if (!texture.create(s.x, s.y))
    {
        std::cout << "error creating texture" << std::endl;
    }
    texture.update(&pixels[0]);
    sprite.setTexture(texture);

    //sf::Image temp_image = other.texture.copyToImage();
    //texture.loadFromImage(temp_image);
    //texture.update(&pixels[0]);
}

RenderFrame& RenderFrame::operator=(RenderFrame other)
{
    if (&other == this)
        return *this;

    std::swap(pixels, other.pixels);
    std::swap(sprite, other.sprite);

    sf::Vector2u s = other.texture.getSize();
    if (!texture.create(s.x, s.y))
        std::cout << "error" << std::endl;
    texture.update(&pixels[0]);
    sprite.setTexture(texture);

    //texture.loadFromMemory()
    //sf::Image temp_image = other.texture.copyToImage();
    //texture.loadFromImage(temp_image);
    //texture.update(&pixels[0]);

    return *this;
}
RenderFrame::RenderFrame(const int width, const int height)
: pixels(4*width*height)
{
    init_texture(sf::IntRect(0, 0, width, height));
}
RenderFrame::RenderFrame(const sf::IntRect& box)
: pixels(4*box.width*box.height)
{
    #warning "deprecated: RenderFrame constructor is based off of external coordinates"
    init_texture(box);
}
RenderFrame::RenderFrame(const PictureType& frame_pixels, const int width, const int height)
: pixels(frame_pixels)
{
    init_texture(sf::IntRect(0, 0, width, height), false);
}
RenderFrame::RenderFrame(const PictureType& frame_pixels, const sf::IntRect& box)
: pixels(frame_pixels)
{
    #warning "RenderFrame constructor is based off of external coordinates"
    init_texture(box, false);
}

void RenderFrame::refreshTexture()
{
    this->texture.update(&pixels[0]);
}
