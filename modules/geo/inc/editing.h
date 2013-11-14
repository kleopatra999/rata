#ifndef HAVE_GEO_EDITING_H
#define HAVE_GEO_EDITING_H

#include "../../vis/inc/common.h"
#include "../../core/inc/window.h"

namespace geo {

    struct Resident_Editor : vis::Drawn<vis::Overlay>, core::Listener {
        void activate ();
        void deactivate ();
        Resident_Editor ();
        ~Resident_Editor ();

        void Drawn_draw (vis::Overlay) override;
        bool Listener_button (int, int) override;
        void Listener_cursor_pos (int x, int y) override;
    };
    extern Resident_Editor* resident_editor;

}

#endif