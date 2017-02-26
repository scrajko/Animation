
#include <OpenGL_common_SFML.hpp>

#include <SFML/Graphics.hpp>

#include <vector>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <random>
#include <ctime>
#include <cmath>

const double Pi = 3.141592653589793238462643383279502884197169399375;

#include <other/Common.hpp>

#include <graphics/Color.hpp>

#include <filtering/Filtering.hpp>

#include <brush/Brush.hpp>
#include <frame/RenderFrame.hpp> // TODO: Render -> Raster
#include <frame/VectorFrame.hpp>
#include <frame/Frame.hpp>

#include <other/Coordinate.hpp>
#include <other/Misc.hpp>
#include <other/Randomization.hpp>

#include <tablet/WinTabFunctionality.hpp>

#include <lodepng/lodepng.h>

void clear_pixels(RawPixelArray& pixels, const Color& color)
{
    for (std::size_t i = 0; i < pixels.size(); i += 4)
    {
        pixels[i+0] = color.r;
        pixels[i+1] = color.g;
        pixels[i+2] = color.b;
        pixels[i+3] = color.a;
    }
}


/**
 * 5. Raster Brush Paint program with SFML!
 */
#define MAIN_PROGRAM    5
#define UNIT_TEST_MODE  0  // 1 == true

// Prevent any other main()'s besides
// the test unit's main (in other/UnitTesting.hpp) from running
// if Unit Test Mode is on:
////////////////////////////////
#if UNIT_TEST_MODE            //
  #include "UnitTesting.hpp"  //
  #undef   MAIN_PROGRAM       //
  #define  MAIN_PROGRAM  0    //
#endif                        //
////////////////////////////////


#if MAIN_PROGRAM == 1
int main() {}

#elif MAIN_PROGRAM == 5 // Paint
/**
 *
 * 5. Raster Brush Paint/Animation program with SFML!
 *
 */

#include <mvc/Paint_View_GUI.hpp>
#include <mvc/Paint_Controller.hpp>

// TODO: Put these in the correct update files
void onSave(const std::vector<RenderFrame>& frames, const sf::IntRect& frame_box)
{
    std::cout << "Saving " << frames.size() << " frames... ";

    std::string file_dir   = "saved/";
    std::string filename   = "files";
    std::string frame_name = "frame";

    unsigned i = 0;
    for (auto& frame : frames)
    {
        std::stringstream ss;
        ss << file_dir + frame_name << i++ << ".png";
        encodeOneStep(ss.str().c_str(), frame.pixels, frame_box.width, frame_box.height);
    }

    std::ofstream f(file_dir+filename);
    f << frames.size();

    std::cout << "Done." << std::endl;
}
void onLoad(std::vector<RenderFrame>& frames, const sf::IntRect& frame_box)
{
    std::cout << "Loading frames... ";

    std::string file_dir   = "saved/";
    std::string filename   = "files";
    std::string frame_name = "frame";

    std::ifstream f(file_dir + filename);
    std::size_t size;
    f >> size;
    frames.resize(size);

    for (std::size_t i = 0; i < size; i++)
    {
        std::stringstream ss;
        ss << file_dir + frame_name << i << ".png";

        unsigned width, height;
        std::vector<unsigned char> pixels = decodeOneStep(ss.str().c_str(), width, height);
        frames[i] = RenderFrame(pixels, frame_box);
    }

    std::cout << "Done" << std::endl;
}

class Unused {
    Unused() { program_assert(false, "Unused variable alert! Self destructing..."); }
};

enum class PressureMode : int {
    Radius  = 0b001,
    Opacity = 0b010,
    Color   = 0b100
};

template <typename T>
bool check_bit(T var, T pos)
{
    return (var) & (1 << (pos));
}

struct PaintInitSettings {
    const std::string path = "./app_data/init.txt";
    PaintInitSettings()
    {
        std::ifstream f(path);
        if (f)
        {
            f >> WindowWidth;
            f >> WindowHeight;
            f >> FrameWidth;
            f >> FrameHeight;
        }
        else
        {
            std::cout << "Error opening "+path+", using defaults" << std::endl;
            WindowWidth  = 1250;
            WindowHeight = 690;
            FrameWidth   = 800;
            FrameHeight  = 600;
        }
    }
    unsigned WindowWidth;
    unsigned WindowHeight;
    unsigned FrameWidth;
    unsigned FrameHeight;
};

int main(int argc, char** argv)
{
    std::cout << "Paint Program, by Sean!\n" << argv[0] << "\n" << std::endl;

    /* Internal state of data */
    std::cout << "Setting up model... " << std::flush;

    PaintInitSettings init_settings;

    PaintModel model(init_settings.FrameWidth, init_settings.FrameHeight);

    GUI_Layout gui_layout (init_settings.WindowWidth, init_settings.WindowHeight,
                           init_settings.FrameWidth, init_settings.FrameHeight);
    GUI gui               (gui_layout);

    std::cout << "view... " << std::flush;
    /* State/functionality of data needed for viewing */
    PaintView view        (init_settings.WindowWidth, init_settings.WindowHeight,
                           init_settings.FrameWidth, init_settings.FrameHeight,
                           model);

    std::cout << "controller... "  << std::flush;
    /* State needed for user input and manipulation of model and view */
    PaintController controller(model, view);
    std::cout << "done. " << std::endl;

    auto& window = view.getWindow();
    view.getWindow().setPosition(sf::Vector2i(42, 10));

    HCTX hCtx = NULL;
    if (!init_tablet(hCtx, window))
    {
        std::cout << "Error initializing tablet functionality" << std::endl;
    }

    int pressure = 0;
    //int old_pressure = pressure;
    while (window.isOpen())
    {
        // TODO: There's still a bug that manifests itself
        // when opacity < 100%, and #define ALPHA_CAP_TEST (in Brush.hpp) is turned off.
        // I'm going to ignore it for now... but I should write a minimum test to try to reproduce it.

        // TODO: Adding in the temp layer
        // made Eraser mode in Brush no longer work.
        // Need to separate the logic more that erase writes directly to current frame pixels.

        static double t = 0;
        t += 0.001;
        if (t > 2 * 3.1415926535)
        {
            t -= 2 * 3.1415926535;
        }

        controller.handleEvents();

        pressure = getPressure(hCtx);

        // At times, the getPressure() function seems to be really slow,
        // which means that the WinTab API is slowing down.
        // Check the Wacom WinTab API--you might just need to clear the buffer now again?
        //
        // Also, the pressure affecting the radius on the spot doesn't
        // mesh well with the spacing algorithm stuff, you'll
        // have to look into that.

        if (pressure == -1) // no packet gotten
        {
            // This is what is causing the problems.
            // We should be using the "WT_PROXIMITY" stuff,
            // to see when pen has entered tablet zone, and not
            // relying on the return falue of the getPackets function.

            model.brush.radius = model.brush.max_radius;
        }
        else
        {
            //PressureMode
            int pressure_mode =
              //static_cast<int>(PressureMode::Opacity) |
              static_cast<int>(PressureMode::Color)   |
              static_cast<int>(PressureMode::Radius);

            // TODO: Change to & logic 001 010 100 (might not be necessary though)
            if (pressure_mode & static_cast<int>(PressureMode::Opacity))
            {
                unsigned char model_brush_color_default_max_alpha = 245; // user can set this, ex: 50%
                model.brush.color.a = model_brush_color_default_max_alpha * (pressure / 2047.0);
            }
            if (pressure_mode & static_cast<int>(PressureMode::Radius))
            {
                model.brush.radius = 0.51 + model.brush.max_radius * (pressure / 2047.0);
            }
            if (pressure_mode & static_cast<int>(PressureMode::Color))
            {
                static const Color color_low_pressure  = model.brush.color;
                static const Color color_high_pressure = Color::Black;

                double t = pressure / 2047.0;
                Byte r = (1.0 - t)*color_low_pressure.r + t * color_high_pressure.r;
                Byte g = (1.0 - t)*color_low_pressure.g + t * color_high_pressure.g;
                Byte b = (1.0 - t)*color_low_pressure.b + t * color_high_pressure.b;
                model.brush.color = Color(r, g, b);
            }
        }

        // mouse positioning/click update is done before keyboard-key updates
        controller.update_fromMouse();
        controller.update /*fromKeyboard*/ ();

        view.draw();
    }
}

#endif // MAIN_PROGRAM
