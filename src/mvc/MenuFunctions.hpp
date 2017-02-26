
#ifndef MENU_FUNCTIONS_HPP
#define MENU_FUNCTIONS_HPP

#include <mvc/Paint_Model.hpp>
#include <mvc/Paint_View_GUI.hpp>

#include <other/Coordinate.hpp>

#define menu_functions_inline inline

#if 0
menu_functions_inline
void menu_FileNew(PaintModel& model, PaintView& view);
menu_functions_inline
void menu_FileSave(PaintModel& model, PaintView& view);
menu_functions_inline
void menu_FileSaveAs(PaintModel& model, PaintView& view);
menu_functions_inline
void menu_FileExit(PaintModel& model, PaintView& view);
menu_functions_inline
void menu_EditUndo(PaintModel& model, PaintView& view);
menu_functions_inline
void menu_EditRedo(PaintModel& model, PaintView& view);
menu_functions_inline
void menu_ToolsBrush(PaintModel& model, PaintView& view);
menu_functions_inline
void menu_ToolsEyedropper(PaintModel& model, PaintView& view);
menu_functions_inline
void menu_ToolsBucket(PaintModel& model, PaintView& view);
menu_functions_inline
void menu_ToolsMove(PaintModel& model, PaintView& view);
menu_functions_inline
void menu_ToolsSquareSelection(PaintModel& model, PaintView& view);
menu_functions_inline
void menu_EffectsCrystallization(PaintModel& model, PaintView& view);
menu_functions_inline
void menu_EffectsBlur(PaintModel& model, PaintView& view);
#else

/* Converts an image to grayscale.
  Mutates the source pixels

  See: https://en.wikipedia.org/wiki/Grayscale
*/
inline
void grayscale(RawPixelArray& pixels, int Height, int Width)
{
    for (int y = 0; y < Height; y++)
    for (int x = 0; x < Width;  x++)
    {
        std::size_t i = RGBA_index(Coordinate(x, y), Width, Height);
        double r = pixels[i + 0] / 255.0;
        double g = pixels[i + 1] / 255.0;
        double b = pixels[i + 2] / 255.0;
        //double a = pixels[i + 3] / 255.0;

        double lum = 0.2126 * r + 0.7152 * g + 0.0722 * b;
        /*
        if (y <= 0.0031308)
            lum = 12.92 * lum;
        else
            lum = 1.055 * std::pow(lum, 1.0 / 2.4) - 0.055;
        */
        if (lum > 1.0)
            std::cout << "lum overflow, lum = " << lum << std::endl;

        pixels[i+0] = 255.0 * lum + 0.5;
        pixels[i+1] = 255.0 * lum + 0.5;
        pixels[i+2] = 255.0 * lum + 0.5;
        //pixels[i+3] = 255.0;

        // TODO: Keep correct transparency
    }
}

menu_functions_inline
void menu_FileNew(PaintModel& model, PaintView& view)
{
    std::size_t i = model.frame_index;
    grayscale(model.raster_frames[i].pixels, model.frame_pixel_width, model.frame_pixel_height);
    model.raster_frames[i].refreshTexture();
}
menu_functions_inline
void menu_FileSave(PaintModel& model, PaintView& view)
{

}
menu_functions_inline
void menu_FileSaveAs(PaintModel& model, PaintView& view)
{

}
menu_functions_inline
void menu_FileExit(PaintModel& model, PaintView& view)
{
    view.getWindow().close();
}
menu_functions_inline
void menu_EditUndo(PaintModel& model, PaintView& view)
{

}
menu_functions_inline
void menu_EditRedo(PaintModel& model, PaintView& view)
{

}
menu_functions_inline
void menu_ToolsBrush(PaintModel& model, PaintView& view)
{

}
menu_functions_inline
void menu_ToolsEyedropper(PaintModel& model, PaintView& view)
{

}
menu_functions_inline
void menu_ToolsBucket(PaintModel& model, PaintView& view)
{

}
menu_functions_inline
void menu_ToolsMove(PaintModel& model, PaintView& view)
{

}
menu_functions_inline
void menu_ToolsSquareSelection(PaintModel& model, PaintView& view)
{

}
menu_functions_inline
void menu_EffectsCrystallization(PaintModel& model, PaintView& view)
{

}
menu_functions_inline
void menu_EffectsBlur(PaintModel& model, PaintView& view)
{

}

#endif

#endif // MENU_FUNCTIONS_HPP

