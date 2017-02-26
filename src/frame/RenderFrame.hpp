
#ifndef RENDER_FRAME_HPP
#define RENDER_FRAME_HPP

#include <SFML/Graphics.hpp>

using PictureType = std::vector<sf::Uint8>; // sf::Uint8 compatible with unsigned char

// Layer Settings
// TODO: this can be passed into brush functions
struct LayerSettings {
    bool transparency_lock;
    bool pixels_lock;
    bool position_lock;
};

// Current problem with RenderFrame:
// sprite's position depends on how current GUI is set up
// Currently avoiding this to full extent by:
// - Always set sprite's position to (0, 0)
// - When drawing, set sprite's position to (left, top),
//   then set back to (0, 0)

/**
 * Raster RenderFrame to contain the pixels
 * for each frame that are drawn to.
 * Current duplicates the information as a texture and
 * as an array of 8bit-color pixels.
 */
struct RenderFrame {
    RenderFrame() {}

    // Construct blank frame:
    RenderFrame(const int width, const int height);
    RenderFrame(const sf::IntRect& box);

    // Construct frame from pixels:
    RenderFrame(const PictureType& frame_pixels, const sf::IntRect& box);
    RenderFrame(const PictureType& frame_pixels, const int width, const int height);

    // Make from another RenderFrame:
    RenderFrame(const RenderFrame& other);
    RenderFrame& operator=(RenderFrame other);

    void refreshTexture();

    PictureType pixels;
    sf::Texture texture;
    sf::Sprite  sprite;

    void init_texture(const sf::IntRect& box, bool clear_pixels = true);
};



#endif // RENDER_FRAME_HPP

