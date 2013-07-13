#ifndef HAVE_VIS_TILES_H
#define HAVE_VIS_TILES_H

#include "../../util/inc/geometry.h"
#include "../../util/inc/organization.h"
#include "../../core/inc/images.h"

namespace vis {
    using namespace util;

    struct Tiles : Linkable<Tiles> {
        uint vbo_id;
        uint vao_id;
        size_t vao_size;
        virtual Vec Tiles_pos () = 0;
        virtual core::Texture* Tiles_texture () = 0;
        void appear ();
        void disappear ();
         // Call this in your own finish
        void finish (uint32 width, uint32 height, const uint16* tiles);
    };

}

#endif