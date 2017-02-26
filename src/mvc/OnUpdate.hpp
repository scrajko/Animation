
#ifndef ON_UPDATE_HPP
#define ON_UPDATE_HPP


#include <iostream>
#include <string>
#include <sstream>
#include <iomanip> //setfill, setw
#include <vector>

#include <cstdlib> // system(), ffmpeg
 // TODO: https://www.securecoding.cert.org/confluence/pages/viewpage.action?pageId=2130132

#include "Paint_Model.hpp"
#include "Paint_View_GUI.hpp"


/**
 * Functions organized by action
 * that manipulate PaintView and PaintModel.
 * Called internally in PaintController update functions
 *
 * The following functions update the model accordingly,
 * while also have the additional role of acting like glue,
 * in order to let the view know it has to update.
 */

// The alternative structure, which isn't bad, would be:
/* if (to_load)
   {
       onLoad(model)
       onLoad(view);
       to_load = false;
   }
*/

// TODO: Merge these redundant functions with the appropriate on____ function
inline
void expand_frame(std::vector<RenderFrame>& frames,
                  std::size_t frame_index,
                  const sf::IntRect& frame_box)
{
    int n;
    std::cout << "Enter expansion number: ";
    std::cin >> n;
    std::cout << "Expanding frame " << frame_index << " to " << n << " frames... ";

    std::vector<RenderFrame>::iterator it = frames.begin();
    frames.insert(it + frame_index, n-1, RenderFrame(frames[frame_index].pixels, frame_box));

    std::cout << "Done." << std::endl;
}

inline
void duplicate_all_frames(std::vector<RenderFrame>& frames)
{
    std::cout << "Duplicating all frames... ";

    std::size_t curr_frame_index = 0;
    std::size_t frames_original_size = frames.size();
    for (std::size_t i = 0; i < frames_original_size; i++)
    {
        // Inefficient due to the number of inserts
        // Adjust size and indices beforehand!
        std::vector<RenderFrame>::iterator it = frames.begin();
        frames.insert(it + curr_frame_index, RenderFrame(frames[curr_frame_index]));
        curr_frame_index += 2;
    }

    std::cout << "Done." << std::endl;
}

// "Saving" should only manipulate the save data folder.
// "Ctrl-Shift-S" could say to write the finalized frames to the finalized_frames folder
// Much easier to apply affects than to edit them, organization-wise...

// save_data folder contains the raw frames
// and necessary config files to organize all the frames, layers, and effects
const std::string save_data_dir_name        = "save_data";

// finalized_frames contains each flattened image,
// to use with FFMPEG or another program to edit further
// or produce a finalized video!
const std::string finalized_frames_dir_name = "finalized_frames";

// TODO: Let's say we have two projects that represent two adjacent scenes
// of a clip. They are each going to have their own frame000-frameXYZ's in them.
// FFMPEG can't put both of them into the save video (or maybe it can with some complicated command.. but anyway)
// Develop a utility project that reads multiple projects
// (you tell it the order of the projects) and it all the images
// to a destination folder that has the correct ordered numbering so that
// FFMPEG can handle it.

// TODO: Separate view from model, only external functions should combine 2x function calls

void onRasterRectangularSelection(PaintModel& model, const sf::Vector2f& converted_curr_pos)
{
    model.rectangular_selection_state.secondary_corner = converted_curr_pos;
}
void onRasterRectangularSelection(const PaintModel& model, PaintView& view, const sf::Vector2f& converted_curr_pos)
{
    view.selection_box.setSize(sf::Vector2f(converted_curr_pos.x - model.rectangular_selection_state.primary_corner.x,
                                            converted_curr_pos.y - model.rectangular_selection_state.primary_corner.y));
}

void onPan(PaintModel& model, const sf::Vector2f& converted_curr_pos)
{
    // Nothing, Model doesn't change
}
void onPan(const PaintModel& model, PaintView& view, const sf::Vector2f& converted_curr_pos)
{
    view.m_camera.move(model.pan_state.origin - converted_curr_pos);
}

inline
void onMove(PaintModel& model)
{
    // TODO: Need to prevent switching modes
    // if user has mouse pressed down.

    if (model.rectangular_selection_state.active)
    {
        // move selection frame:
        model.rectangular_selection_state.floating_selection.sprite.move(1, 1);

        // move selection rectangle along with it:
        //model.rectangular
    }

}
inline
void onMove(const PaintModel& model, PaintView& view)
{
    // TODO Not implemented

    // move current cursor position - mouse click original position
    /*if (holding selection box)
    {
        move view's rectangle shape
    }*/
}


/**
 * Process what should happen when the user clicks inside the program
 * Goes through the state logic to determine what action or tool to use.
 * model.mouse_state.mouse_coord (and old_mouse_coord) should be updated before this function call!
 */
inline
void onMousePress(PaintModel& model, const sf::Vector2f& converted_mouse_state_mouse_coord)
{
    auto& mouse_state = model.mouse_state;
    //auto& brush       = model.brush;
    auto& frames      = model.raster_frames;
    auto& frame_index = model.frame_index;
    auto& tool_mode   = model.tool_mode;

    mouse_state.left_button_pressed      = true;
    mouse_state.left_button_just_pressed = true;

    // moved from update_fromMouse()
    // Potential problem: left_button_just_pressed is used both here
    // and in update_fromMouse()...
    // should left_button_just_pressed be reset here
    // I just got rid of the (mouse_state.left_button_just_pressed) condition since
    // it would have to be true...

    // Notice: I am using the converted pixel coords here,
    // since let's say you're zoomed out really far and 1 screen pixel
    // is 100 frame pixels, you would still want it to fill in all of those in betweens pixels
    // In other words: The spacing should be dependent on number of texture
    // pixels difference, not the actual screen pixels difference.

    if (tool_mode == ToolMode::Brush || tool_mode == ToolMode::Eraser)
    {
        // TODO: name change to "last contact coord"?
        // Brush itself shouldn't have state, there should be BrushState struct
        model.brush.previous_position = converted_mouse_state_mouse_coord;
        model.brush_distance = 0.0;

        //std::cout << "Saving pixel undo backup..." << std::endl;
        model.backup_pixels = frames[frame_index].pixels;
        model.undo_frame_index = frame_index;
    }
    else if (tool_mode == ToolMode::RectangularSelection)
    {
        if (model.rectangular_selection_state.active)
        {
            model.rectangular_selection_state.active = false;

            // Merge the currently-select pixels back into the layer they belong to:
            // Note: When I start to use layers, how does Photoshop handle
            // making a selection, moving it while switching layers
            auto& floating_selection = model.rectangular_selection_state.floating_selection;
            auto& frame              = model.raster_frames[model.frame_index];
            auto& frame_width  = model.frame_pixel_width;
            auto& frame_height = model.frame_pixel_height;

            // TODO: Like in the other function, this stuff should be pixel aligned...
            int selection_width    = floating_selection.sprite.getGlobalBounds().width;
            int selection_height   = floating_selection.sprite.getGlobalBounds().height;
            int selection_offset_x = floating_selection.sprite.getGlobalBounds().left;
            int selection_offset_y = floating_selection.sprite.getGlobalBounds().top;

            for (int y = 0; y < selection_height; y++)
            for (int x = 0; x < selection_width; x++)
            {
                std::size_t frame_index     = RGBA_index(
                  Coordinate(x + selection_offset_x, y + selection_offset_y), frame_width, frame_height);
                std::size_t selection_index = RGBA_index(
                  Coordinate(x, y),        selection_width, selection_height);

                frame.pixels[frame_index+0] = floating_selection.pixels[selection_index+0];
                frame.pixels[frame_index+1] = floating_selection.pixels[selection_index+1];
                frame.pixels[frame_index+2] = floating_selection.pixels[selection_index+2];
                frame.pixels[frame_index+3] = floating_selection.pixels[selection_index+3];
            }
        }

        model.rectangular_selection_state.primary_corner   = converted_mouse_state_mouse_coord;
        model.rectangular_selection_state.secondary_corner = converted_mouse_state_mouse_coord;
    }
    else if (tool_mode == ToolMode::Move)
    {
        if (model.rectangular_selection_state.active)
        {
            // TODO: Implement moving a rectangle selection
            /*
            if (selection.getGlobalBounds().contains(converted_mouse_state_mouse_coord))
            {
                holding_selection = true;
            }*/
        }
    }
    else if (tool_mode == ToolMode::Eyedropper)
    {
        auto& frame_width  = model.frame_pixel_width;
        auto& frame_height = model.frame_pixel_height;
        auto& pixels = frames[frame_index].pixels;

        // TODO The pixel selected is rounded incorrectly,
        // seems to have a fencepost error somewhere.
        // This also probably apply to all the click-based functionality.

        // Also of importance:
        // In Photoshop, when you use the Eyedropper tool, all transparency information is taken away.
        // This would the equivalent of merging it with a white pixel
        // on the bottom and then taking the color. That sounds easy enough,
        // could be toggle..able in a config file/menu.

        std::size_t i = RGBA_index(rounding(Coordinate(converted_mouse_state_mouse_coord.x,
                                                       converted_mouse_state_mouse_coord.y)), frame_width, frame_height);
        model.brush.color = Color(pixels[i+0], pixels[i+1], pixels[i+2], pixels[i+3]);
    }
    else if (tool_mode == ToolMode::Pan)
    {
        // TODO: If we implement the "temporary mode" stuff like
        // holding down Space bar for Hand/Pan,
        // we'll need to keep a save state of the previous tool
        // to switch back to.


    }

    // TODO: Menu clicking shouldn't overwrite anything...
    //  It should be clear that this function is only called after we know
    //  that the click was not in a menu.
    // It should check for the ToolMode first
}

inline
void onMousePress(const PaintModel& model, PaintView& view, const sf::Vector2f& converted_mouse_state_mouse_coord)
{
    if (model.tool_mode == ToolMode::RectangularSelection)
    {
        view.selection_box.setPosition(converted_mouse_state_mouse_coord);
    }
}


// Called from onMouseRelease
// TODO: Rename
inline
void _onMerge(PaintModel& model)
{
    auto& raster_frame  = model.raster_frames[model.frame_index];
    unsigned width      = model.frame_pixel_width;
    unsigned height     = model.frame_pixel_height;

    // update raster_frame, zero the premerged_layer
    merge_layers(model.premerged_brush_layer_pixels, raster_frame.pixels, width, height);

    // Update the textures of both the now-zeroed premerged layer and the newly merged raster_frame
    model.premerged_brush_layer_texture.update(&model.premerged_brush_layer_pixels[0]);
    raster_frame.refreshTexture();
}

inline
void onMouseRelease(PaintModel& model)
{
    auto& tool_mode = model.tool_mode;
    model.mouse_state.left_button_pressed = false;
    if (model.temp_layer_painted)
    {
        _onMerge(model);
        model.temp_layer_painted = false;
    }

    if (tool_mode == ToolMode::RectangularSelection)
    {
        if (model.rectangular_selection_state.primary_corner !=
             model.rectangular_selection_state.secondary_corner)
        {

            // copy only the rectangular selection:
            // TODO: Rounding issues
            // Selections should be aligned with pixels
            int x_min = std::min(model.rectangular_selection_state.primary_corner.x,
                                 model.rectangular_selection_state.secondary_corner.x);
            int x_max = std::max(model.rectangular_selection_state.primary_corner.x,
                                 model.rectangular_selection_state.secondary_corner.x);
            int y_min = std::min(model.rectangular_selection_state.primary_corner.y,
                                 model.rectangular_selection_state.secondary_corner.y);
            int y_max = std::max(model.rectangular_selection_state.primary_corner.y,
                                 model.rectangular_selection_state.secondary_corner.y);


            // 0-2 -> [0, 1, 2] -> 3 pixels
            std::size_t selection_width  = (x_max - x_min + 1);
            std::size_t selection_height = (y_max - y_min + 1);

            RawPixelArray selection_pixels(4 * selection_width * selection_height);
            RawPixelArray& frame_pixels = model.raster_frames[model.frame_index].pixels;
            auto& frame_width  = model.frame_pixel_width;
            auto& frame_height = model.frame_pixel_height;

            for (int y = y_min; y <= y_max; y++)
            for (int x = x_min; x <= x_max; x++)
            {
                std::size_t frame_index     = RGBA_index(Coordinate(x, y),                 frame_width,     frame_height);
                std::size_t selection_index = RGBA_index(Coordinate(x - x_min, y - y_min), selection_width, selection_height);
                selection_pixels[selection_index+0] = frame_pixels[frame_index+0];
                selection_pixels[selection_index+1] = frame_pixels[frame_index+1];
                selection_pixels[selection_index+2] = frame_pixels[frame_index+2];
                selection_pixels[selection_index+3] = frame_pixels[frame_index+3];

                // make the underneath transparent:
                frame_pixels[frame_index+0] = 0;
                frame_pixels[frame_index+1] = 0;
                frame_pixels[frame_index+2] = 0;
                frame_pixels[frame_index+3] = 0;
            }
            model.raster_frames[model.frame_index].refreshTexture();

            model.rectangular_selection_state.floating_selection = RenderFrame(selection_pixels, selection_width, selection_height);
            model.rectangular_selection_state.floating_selection.sprite.setPosition(model.rectangular_selection_state.primary_corner);

            model.rectangular_selection_state.active = true;
        }
        else
        {
            model.rectangular_selection_state.active = false;
        }
    }
}

inline
void onMouseRelease(const PaintModel& model, PaintView& view)
{
    auto& tool_mode = model.tool_mode;
    if (tool_mode == ToolMode::Brush || tool_mode == ToolMode::Eraser)
    {

    }
    else if (tool_mode == ToolMode::RectangularSelection)
    {

    }
}


///////////////////////////////////////////////////////
///  Functions that still depend on View and Model  ///
///////////////////////////////////////////////////////

inline
void onZoomIn(PaintModel& model, PaintView& view)
{
    view.m_camera.zoom(0.95);
}
inline
void onZoomOut(PaintModel& model, PaintView& view)
{
    view.m_camera.zoom(1.05);
}
inline
void onPlus(PaintModel& model, PaintView& view)
{
    auto& brush = model.brush;
    int frame_width = model.frame_pixel_width;

    brush.max_radius++;
    if (brush.max_radius > frame_width/2)
        brush.max_radius = frame_width/2;

    //#warning "outdated circle update"
    //update_brush_circle(brush_circle, brush);
}
inline
void onMinus(PaintModel& model, PaintView& view)
{
    auto& brush = model.brush;
    brush.max_radius--;
    if (brush.max_radius < 0)
        brush.max_radius = 0;
    //#warning "outdated circle update"
    //update_brush_circle(brush_circle, brush);
}
inline
void onLeft(PaintModel& model, PaintView& view)
{
    auto& frame_index = model.frame_index;

    if (frame_index != 0)
        frame_index--;

    std::cout << "frame " << frame_index << std::endl;
}
inline
void onRight(PaintModel& model, PaintView& view)
{
    auto& frames = model.raster_frames;
    auto& vector_frames = model.vector_frames;

    auto& frame_index = model.frame_index;
    int width  = model.frame_pixel_width;
    int height = model.frame_pixel_height;

    // I'm trying to decide what to with the Raster Layer vs the Vector layer.
    // What should the relation between the raster layer and the vector layer be?
    // Should the vector layer copy the raster layer (way too many point would be generated in practice, and how would you erase points?)
    // Should the raster layer copy the vector layer on command?
    // Should "Paths" be part of the vector layer or their own separate thing?
    // Should we have a regular drawing layer,
    // and then the dual raster+vector layer, where the raster is updated from the vector?

    frame_index++;
    if (frame_index == frames.size())
    {
        frames.push_back(RenderFrame(width, height));
        vector_frames.push_back(VectorFrame());

    }
    std::cout << "frame " << frame_index << std::endl;
}
inline
void onSave(PaintModel& model, PaintView& view)
{
    // TODO: Save using the newly decided folder organization
    // Also save vector info, layer info, etc.
    // TODO: Combine redundant information into a struct (like "files")

    auto& frames = model.raster_frames;
    int width  = model.frame_pixel_width;
    int height = model.frame_pixel_height;

    std::cout << "Saving " << frames.size() << " frames... " << std::flush;

    std::string path = "projects/" + model.project_name + "/" + save_data_dir_name;
    std::string frame_name = model.project_name;
    std::string config_filename = "files";

    unsigned i = 0;
    for (auto& frame : frames)
    {
        std::stringstream ss;
        ss << path << "/" << frame_name << i++ << ".png";
        encodeOneStep(ss.str().c_str(), frame.pixels, width, height);
    }

    std::ofstream f(path + "/" + config_filename);
    f << frames.size();

    std::cout << "Done." << std::endl;
}

/**
 * Saves the current frame as a picture,
 * so that the whole animation or whatever doesn't have to be saved
 */
inline
void onSaveAlt(PaintModel& model, PaintView& view)
{
    auto& frame_index = model.frame_index;
    auto& frame = model.raster_frames[frame_index];
    int width   = model.frame_pixel_width;
    int height  = model.frame_pixel_height;

    std::cout << "Saving frame " << frame_index << " individually... " << std::flush;

    std::string path = "singles";
    std::string frame_name = model.project_name;
    std::string file_count_filename = "file_count";

    // Update the number of files that are in the folder:
    std::fstream f(path + "/" + file_count_filename);
    if (!f)
    {
        std::cout << "Error opening " << path + "/" + file_count_filename << std::endl;
    }
    std::size_t count;
    f >> count;
    f.close();
    f.open(path + "/" + file_count_filename);
    if (!(f << count + 1)) {std::cout << "ERRRORRR" << std::endl;}

    std::stringstream ss;
    ss << path << "/" << frame_name << count << ".png";
    encodeOneStep(ss.str().c_str(), frame.pixels, width, height);

    std::cout << "Done." << std::endl;
}

inline
void onLoad(PaintModel& model, PaintView& view)
{
    auto& frames = model.raster_frames;
    auto& frame_index = model.frame_index;

    std::cout << "Loading frames... " << std::flush;

    std::string path = "projects/" + model.project_name + "/" + save_data_dir_name;
    std::string frame_name = model.project_name;
    std::string config_filename = "files";

    std::ifstream f(path + "/" + config_filename);
    std::size_t size;
    f >> size;
    frames.resize(size);

    for (std::size_t i = 0; i < size; i++)
    {
        std::stringstream ss;
        ss << path << "/" << frame_name << i << ".png";

        unsigned width, height;
        std::vector<unsigned char> pixels = decodeOneStep(ss.str().c_str(), width, height);
        frames[i] = RenderFrame(pixels, sf::IntRect(0, 0, width, height));

        // TODO: Properly set width and height properties in model
    }
    frame_index = 0; // Put user at first frame of loaded scene

    std::cout << "Done." << std::endl;
}

#include <frame/FrameProcessing.hpp>

inline
void onExport(PaintModel& model, PaintView& view)
{
    // At the end of the day, what we're left with
    // is a list of PNG pixel frames all strewn together.
    // So it does make more sense to have a list of frames,
    // and these frames then have subsections like Layers, each which contain a "raster layer" "vector layer"
    // Frame { int duration; }
    auto& frames = model.raster_frames;
    int width  = model.frame_pixel_width;
    int height = model.frame_pixel_height;

    std::cout << "Exporting frames... " << std::flush;
    std::string path = "projects/" + model.project_name + "/" + finalized_frames_dir_name;
    std::string frame_name = model.project_name;

    static RawPixelArray flattened_background(4*width*height);

    // TODO: Enable layering and flatten all layers,
    // also combining any other necessary, separate info.

    // TODO: Do more unit tests to ensure that calling stuff like
    // onExport doesn't change the content of the frames.

    unsigned i = 0;
    for (auto& frame : frames)
    {
        //export_frame(frame, i);
        // TODO: in future do the above

        // Instead of zeroing the top layer, we need to zero the bottom layer
        // before we encode it into an image file.
        for (std::size_t i = 0; i < flattened_background.size(); i += 4)
        {
            flattened_background[i+0] = 255; //0;
            flattened_background[i+1] = 255; //0;
            flattened_background[i+2] = 255; //0;
            flattened_background[i+3] = 255;
        }

        merge_layers(frame.pixels, flattened_background, width, height, false);

        add_frame_counter(flattened_background, i, width, height);

        std::stringstream ss;
        ss << path << "/" << frame_name << std::setfill('0') << std::setw(3) << i << ".png";
        encodeOneStep(ss.str().c_str(), flattened_background, width, height);

        i++;
    }

    int framerate = 12;

    // TODO: -vframes is not correctly working
    // what is -framerate vs -r?
    // .mp4 file looks fine but the .webm seems to cut off early...
    // Until I figure this stuff out, just manually delete the files in the finalized_frames folder
    // until I do this.

    // TODO: Conversion to WebM.
    std::stringstream ffmpeg_command;

    /////////////////////////////
    ///  Raw frames to .webm  ///
    /////////////////////////////
    /// CURRENTLY NOT WORKING PROPERLY AT LOW FRAME RATES!!!
    #if 0
      int quality = 20;
      ffmpeg_command << "ffmpeg -y -framerate " << framerate << " -i " << path+"/"+frame_name << "%03d.png ";
      ffmpeg_command << "-an -c:v libvpx -r 30 -crf " << quality << " -b:v 1M -c:a libvorbis " << path+"/"+frame_name << ".webm";
      std::cout << "\n FFMPEG command = " << ffmpeg_command.str() << std::endl;
      system(ffmpeg_command.str().c_str());
    #endif


    ////////////////////////////
    ///  Raw frames to .mp4  ///
    ////////////////////////////
    #if 1
      ffmpeg_command << "ffmpeg -y -start_number 0 -framerate " << framerate << " -i " << path+"/"+frame_name << "%03d.png ";
      ffmpeg_command << /* "-vframes " << frames.size() << */ " -c:v libx264 -r 30 -pix_fmt yuv420p ";
      //ffmpeg_command << "ffmpeg -framerate " << framerate << " -i " << path+"/"+frame_name << "%03d.png -c:v libx264 -pix_fmt yuv420p ";
      ffmpeg_command << path+"/"+frame_name << ".mp4";
      std::cout << "FFMPEG command = " << ffmpeg_command.str() << std::endl;
      system(ffmpeg_command.str().c_str());
      //system("ffmpeg -framerate 1/5 -i img%03d.png -c:v libx264 -r 30 -pix_fmt yuv420p out.mp4");

      ffmpeg_command.str("");
      ffmpeg_command.clear();

     ///////////////////////
     ///  .mp4 to .webm  ///
     ///////////////////////
     // https://github.com/WebMBro/WebMConverter/blob/master/MainForm.cs
     // This command works withh ffmpeg: https://trac.ffmpeg.org/wiki/Encode/VP8 (libpvx is already installed)
     // The command is mp4 -> webm, so both commands must be run.

     int quality = 15;
     ffmpeg_command << "ffmpeg -y -i " << path+"/"+frame_name + ".mp4 -an -c:v libvpx -crf " << quality << " -b:v 1M -c:a libvorbis " << path+"/"+frame_name << ".webm";
     system(ffmpeg_command.str().c_str());
    #endif // raw frames -> mp4 -> webm

    std::cout << "Done." << std::endl;
}
inline
void onDelete(PaintModel& model, PaintView& view)
{
    auto& frames = model.raster_frames;
    auto& frame_index = model.frame_index;

    if (frames.size() == 1)
        std::cout << "Cannot delete last frame";
    else
    {
        std::vector<RenderFrame>::iterator it = frames.begin();
        frames.erase(it + frame_index);
        if (frame_index == frames.size())
            frame_index--;
    }
}

inline
void onInsert(PaintModel& model, PaintView& view, bool insert_pre)
{
    auto& frames = model.raster_frames;
    auto& frame_index = model.frame_index;
    int width  = model.frame_pixel_width;
    int height = model.frame_pixel_height;

    std::vector<RenderFrame>::iterator it = frames.begin();
    if (insert_pre)
    {
        // Inserts the new frame *before* the current frame being shown
        // Ex: If you're on Frame 0 and insert-pre, Frame 0 becomes Frame 1,
        //     new Frame 0 becomes new blank
        frames.insert(it + frame_index, RenderFrame(width, height));
    }
    else // insert post
    {
        // Inserts the new frame *after* the current frame being shown
        frames.insert(it + frame_index + 1, RenderFrame(width, height));
    }
}

/**
 * Assumes selection coordinates will
 * be within frame coordintes. Photoshop doesn't let
 * a selection not be.
 *
 * Concern: If user presses Ctrl-C while still manipulating the
 * selection... guess that should be part of the hasActiveSelection() checks.
 */
inline
void onCopy(PaintModel& model, PaintView& view)
{
    std::cout << "Copying..." << std::endl;

    // New plan: Figure out copying selections after they have been selected.

    /*

    if (model.rectangular_selection_state.active)
    {
        // copy only the rectangular selection:
        // TODO: Rounding issues
        int x_min = std::min(model.rectangular_selection_state.primary_corner.x,
                             model.rectangular_selection_state.secondary_corner.x);
        int x_max = std::max(model.rectangular_selection_state.primary_corner.x,
                             model.rectangular_selection_state.secondary_corner.x);
        int y_min = std::min(model.rectangular_selection_state.primary_corner.y,
                             model.rectangular_selection_state.secondary_corner.y);
        int y_max = std::max(model.rectangular_selection_state.primary_corner.y,
                             model.rectangular_selection_state.secondary_corner.y);

        // Selections should be aligned with pixels

        // 0-2 -> [0, 1, 2] -> 3 pixels
        std::size_t selection_width  = (x_max - x_min + 1);
        std::size_t selection_height = (y_max - y_min + 1);

        RawPixelArray selection_pixels(4 * selection_width * selection_height);
        const auto& frame_pixels = frames[frame_index].pixels

        for (int y = y_min; y <= y_max; y++)
        for (int x = x_min; x <= x_max; x++)
        {
            std::size_t frame_index     = RGBA_index(Coordinate(x, y), ImageWidth, ImageHeight);
            std::size_t selection_index = RGBA_index(Coordinate(x, y), selection_width, selection_height);
            selection_pixels[selection_index+0] = frame_pixels[frame_index+0];
            selection_pixels[selection_index+1] = frame_pixels[frame_index+1];
            selection_pixels[selection_index+2] = frame_pixels[frame_index+2];
            selection_pixels[selection_index+3] = frame_pixels[frame_index+3];
        }
        model.rectangular_selection_state.floating_selection copied_selection = RenderFrame(selection_pixels, selection_width, selection_height);

        // When a user makes a selection, if they use the move tool,
        // they can then move this selection as the "floating selection".


        // also TODO: If user clicks while in selection mode
        // but then doesn't move before lifting up, that should cancel any selections made

    }
    else
    {
        copied_selection = frames[frame_index]; //frames[frame_index].pixels;
    }

    */

}
inline
void onPaste(PaintModel& model, PaintView& view)
{
    auto& frames        = model.raster_frames;
    auto& frame_index   = model.frame_index;
    auto& copied_pixels = model.copy_state;
    int width           = model.frame_pixel_width;
    int height          = model.frame_pixel_height;


    // Need to restructure this whole thing.
    // When a user pastes, we're going to have it do similar thing as MS Paint,
    // it pastes the rectangle as a float selection at the top-right corner of the screen.
    // so this needs to overwrite whatever was being selected.
    //
    // When user is in RectangularSelection mode and releases the mouse,
    // is that when the new RenderFrame should be created?
    // or should the renderframe be created when user presses down while in Move mode.
    //
    // This has to be kept in a separate storage than the Copy storage.
    // As in the floating selection "frame" has to be stored when moving.


    // Be sure to do unit testing with copying...
    //ex: color border pixels that should be copied in advance.
    //   click at (0, 0), lift at (19, 9) -> directly put copy buffer
    //   to PNG file and see result.

    // TODO: For right now, we're simply overwriting
    // But we SHOULD be doing opacity checking
    std::cout << "Pasting..." << std::flush;
    frames[frame_index] = RenderFrame(copied_pixels, width, height);
    std::cout << " Done." << std::endl;
}
inline
void onExpand(PaintModel& model, PaintView& view)
{
    auto& frames = model.raster_frames;
    auto& frame_index = model.frame_index;

    // TODO: Get rid of deprecated constructor
    sf::IntRect frame_box(0, 0, model.frame_pixel_width, model.frame_pixel_height);

    expand_frame(frames, frame_index, frame_box);
}
inline
void onUndo(PaintModel& model, PaintView& view)
{
    auto& frames = model.raster_frames;
    auto& pixels_for_redo = model.pixels_for_redo;
    auto& undo_frame_index = model.undo_frame_index;
    auto& backup_pixels = model.backup_pixels;

    int width           = model.frame_pixel_width;
    int height          = model.frame_pixel_height;

    // If we want to be like Photoshop, add '&& mouse is not pressed'
    std::cout << "Undoing last change..." << std::endl;

    // TODO: Need to prevent undoing twice
    // and overwriting the pixels that are supposed to be saved
    // for redoing
    // TODO: Also, implement this as a vector of undos
    // for multiple undos/redos
    pixels_for_redo = frames[undo_frame_index].pixels;
    frames[undo_frame_index] = RenderFrame(backup_pixels, width, height);
}
inline
void onRedo(PaintModel& model, PaintView& view)
{
    auto& frames = model.raster_frames;
    auto& undo_frame_index = model.undo_frame_index;
    auto& pixels_for_redo = model.pixels_for_redo;
    int width           = model.frame_pixel_width;
    int height          = model.frame_pixel_height;

    std::cout << "Redoing the undo...";
    frames[undo_frame_index] = RenderFrame(pixels_for_redo, width, height); // constructor needed to internally update texture
}
inline
void onRun(PaintModel& model, PaintView& view)
{

}
inline
void onDuplicate(PaintModel& model, PaintView& view)
{
    auto& frames = model.raster_frames;
    duplicate_all_frames(frames);
}

#endif // ON_UPDATE_HPP
