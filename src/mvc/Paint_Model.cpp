
#include "Paint_Model.hpp"

#include <iostream>

PaintModel::PaintModel(unsigned FrameWidth, unsigned FrameHeight)
: frame_pixel_width(FrameWidth),
  frame_pixel_height(FrameHeight),
  raster_frames {RenderFrame(FrameWidth, FrameHeight)} ,  // {RenderFrame(frame_box)}
  vector_frames(1),
  frame_index(0),
  tool_mode(ToolMode::Brush),

  //copied_pixels  (4*FrameWidth*FrameHeight),
  backup_pixels  (4*FrameWidth*FrameHeight),
  pixels_for_redo(4*FrameWidth*FrameHeight),

  premerged_brush_layer_pixels(4*FrameWidth*FrameHeight),
  premerged_brush_layer_texture(),
  premerged_brush_layer_sprite(),
  temp_layer_painted(false),

  undo_frame_index(0),

  #if 0
  brush(4, "brushes/graphic_pen.png"),
  #else
  brush(4),
  #endif

  brush_distance(0.0),

  interpolation_points(),
  project_name("test_project"),

  frame_durations {1},

  mouse_state(),
  pan_state(),
  rectangular_selection_state()
{
    if (!premerged_brush_layer_texture.create(frame_pixel_width, frame_pixel_height))
        std::cout << "Error" << std::endl;
    premerged_brush_layer_sprite.setTexture(premerged_brush_layer_texture);
    vector_frames[frame_index].add_point( VectorRenderPoint(Coordinate(100, 100), 20, VectorRenderStyle(Color::Red)) );
}
