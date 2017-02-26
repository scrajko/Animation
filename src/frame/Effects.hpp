
#ifndef EFFECTS_HPP
#define EFFECTS_HPP

// Effects are hard-coded in

/**
 * Effects class contains list of Textures
 * for each corresponding.
 *
 * The Effect in each frame points to this static list of textures,
 * so that each frame knows how to draw it.
 *
 * This also brings in a divergence of two ways to save things:
 * - Save it as a project for editing later (layers are separated)
 * - Release it as the frames (layers are combined)
 *
 * Also, a good way to keep YouTube quality might be to
 * import the FFMPEG'd video into Adobe Premiere, and then
 * encode it for YouTube from Premiere.
 */

#include <RenderFrame>
#include <vector>

/**
 * Linear transition effects that affect
 * a specified range of Frames,
 * must apply to at least 2 frames!
 *
 * What if we want to combine two effects?
 * ex: Have it go from black to picture, but also go from blurry to crisp as it gets lighter
 * (ex: Waking up from being knocked out? or vice versa if you're about to go out cold?)
 */
enum class TransitionEffect {

    FadeToWhite,
    FadeToBlack,

    // Fade to a custom solid color:
    FadeToColor,
};


/*
 * Effects that can apply to independent frames
 */
enum class Effect {

    Invert,
    TODO
};

class TransitionEffect_TextureHolder {

    // This won't work as intended as of right now
    // The point of the Effects are that they
    // gradually

    // Dynamically allocate a unique pointer for
    // each texture type?
    // - Still has the downside of repeating redundant
    // - Only way to solve that would be to have a "pool" of available
    //     time instances, and log(n) access time if you implement a tree, otherwise (n) time.

    std::vector<sf::Texture> textures;
    std::vector<sf::Sprite> sprites;
};

/**
 * Assumes start_index < end_index
 *
 * start_index frame will contain the "t = 0.0" state of the effect.
 * end_index   frame will contain the "t = 1.0" state of the effect.
 *
 * ex: (Blur, frames, 0, 5):
 * [0]     - Completely blurry
 * [1]-[4] - Becoming less blurry
 * [5]     - Completely unblurry
 *
 */
void addTransitionEffect(TransitionEffect effect, std::vector<RenderFrame& frames,
                           std::size_t start_index, std::size_t end_index)
{
    std::size_t num_frames = end_index - start_index + 1;
    program_assert(num_frames >= 2, "num_frames < 2!");

    for (std::size_t i = start_index; i <= end_index; i++)
    {
        frames[i].transition_effect.type = effect;
        frames[i].transition_effect.t    = static_cast<double>(i - start_index) / (num_frames - 1)
    }
}

#endif // EFFECTS_HPP
