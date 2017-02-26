
#ifndef PAINT_CONTROLLER_HPP
#define PAINT_CONTROLLER_HPP

#include "Paint_Model.hpp"
#include "Paint_View_GUI.hpp"

#define MOUSE_SYSTEM_IMMEDIATE 1

class PaintController {

  public:
    PaintController(PaintModel& model, PaintView& view)
    : m_model(model),
      m_view(view),

      to_plus                (false),
      to_minus                (false),

      to_left                 (false),
      to_right                (false),

      to_zoom_in              (false),
      to_zoom_out             (false),

    // Saves/loads files at "saved" folder
      to_save                 (false),
      to_save_alt             (false),
      to_load                 (false),

      to_export               (false),

    // Copy current frame pixel by pixel to reserved "clipboard"
      to_copy                 (false),

    // Makes incoming pixels be the top layer,
    // and existing pixels be the bottom layer,
    // oombines the pixels using the opacity function.
    // Note: Does not insert a frame, use Insert + Copy for that
      to_paste                (false),

    // Insert new frame before current one being viewed
      to_insert_pre           (false),
    // Insert new frame after current one being viewed
      to_insert_post          (false),

    // Expands current frame to be expanded to N frames that user types in
    // ex: [0, 1, 2, 3 4] -> current frame is 2
    // expand(3) -> makes frame 2 now be 3 independent frames
    //  -> [0, 1, 2, 2, 2, 3, 4]
    //  -> [0, 1, 2, 3, 4, 5, 6]
      to_expand               (false),

    // Deletes the current frame
      to_delete                (false),

    // Can only undo once
      to_undo                  (false),
      to_redo                  (false),

      to_run                   (false),

      to_duplicate             (false),

      to_mouse_press           (false),
      to_mouse_release         (false)

    {}

    void handleEvents(); // sf::Event loop

    void update_fromMouse();
    void update();

  private:
    void onDrawingAreaClick();

  private:
    PaintModel& m_model;
    PaintView&  m_view;

  private:

    bool to_plus;
    bool to_minus;

    bool to_left; // Left arrow pressed
    bool to_right; // Right arrow pressed

    bool to_zoom_in;
    bool to_zoom_out;

    // Saves/loads files at "saved" folder
    bool to_save;
    bool to_save_alt;
    bool to_load;

    // Produce the finalized frames to be used with FFMPEG
    bool to_export;

    // Copy current frame pixel by pixel to reserved "clipboard"
    bool to_copy;

    // Makes incoming pixels be the top layer,
    // and existing pixels be the bottom layer,
    // oombines the pixels using the opacity function.
    // Note: Does not insert a frame, use Insert + Copy for that
    bool to_paste;

    // Insert new frame before current one being viewed
    bool to_insert_pre;
    // Insert new frame after current one being viewed
    bool to_insert_post;

    // Expands current frame to be expanded to N frames that user types in
    // ex: [0, 1, 2, 3 4] -> current frame is 2
    // expand(3) -> makes frame 2 now be 3 independent frames
    //  -> [0, 1, 2, 2, 2, 3, 4]
    //  -> [0, 1, 2, 3, 4, 5, 6]
    bool to_expand;

    // Deletes the current frame
    bool to_delete;

    // Can only undo once
    bool to_undo;
    bool to_redo;

    bool to_run;

    bool to_duplicate;

    bool to_mouse_press;
    bool to_mouse_release;
};

void onRasterDrawing(PaintModel& model, sf::Vector2f converted_old_pos,
                                        sf::Vector2f converted_curr_pos);


#endif // PAINT_CONTROLLER_HPP
