#ifndef HAVE_VIS_GRAFFITI_H
#define HAVE_VIS_GRAFFITI_H

#include "../../util/inc/Vec.h"

namespace vis {

     // Do this after graffiti layer has run
    void draw_line (Vec a, Vec b, uint32 color, float width = 1);
    void draw_chain (uint n_pts, Vec* pts, uint32 color, float width = 1);
    void draw_loop (uint n_pts, Vec* pts, uint32 color, float width = 1);
    void draw_primitive (uint type, uint n_pts, Vec* pts, uint32 color);
    void shift_graffiti (Vec new_pos);  // modifies MODELVIEW matrix

}

#endif