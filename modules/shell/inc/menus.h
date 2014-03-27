#ifndef HAVE_GEO_MENUS_H
#define HAVE_GEO_MENUS_H

#include <string>
#include <vector>
#include <memory>
#include "core/inc/commands.h"
#include "core/inc/window.h"
#include "vis/inc/common.h"
namespace vis { struct Font; }

namespace shell {
    using namespace util;

     // Menu Items are always positioned by their lower-left corner.
     // Individual items do not get to choose what layer they are drawn on.
    struct Menu_Item {
        virtual Vec Menu_Item_size (Vec area) { return Vec(0, 0); }
        virtual void Menu_Item_draw (Vec pos, Vec area) = 0;
         // If these return false, the event is cascaded up.
         // pos is relative to the item's lower-left corner.
        virtual bool Menu_Item_hover (Vec pos, Vec area) { return false; }
        virtual bool Menu_Item_click (Vec pos, Vec area) { return false; }
    };

    extern uint64 last_hover_frame;

    struct Menu_Base : core::Listener {
        Vec pos = Vec(0, 0);
        Vec size = Vec(INF, INF);
        bool deactivate_on_click = false;
        std::unique_ptr<Menu_Item> root;
        void draw ();

        void hover (Vec pos);
        bool click (Vec pos);

        virtual Vec decoord (int x, int y) = 0;
        virtual void activate () = 0;
        virtual void deactivate () = 0;
        bool Listener_event (SDL_Event*) override;
    };
    template <class Layer>
    struct Menu : Menu_Base, vis::Drawn<Layer> {
        void Drawn_draw (Layer) override { draw(); }
        void activate () override {
            last_hover_frame = core::window->frames_drawn;
            vis::Drawn<Layer>::appear();
            core::Listener::activate();
        }
        void deactivate () override {
            vis::Drawn<Layer>::disappear();
            core::Listener::deactivate();
        }
        Vec decoord (int x, int y) override {
            return vis::camera->window_to_layer<Layer>(x, y);
        }
    };

    struct Button : Menu_Item {
        core::Command on_click;
        uint32 color = 0xffffffff;
        uint32 background_color = 0x00000000;
        uint32 hover_color = 0xffffffff;
        uint32 hover_background_color = 0x00000000;
        uint64 hovering_frame = -1;
        bool hovering () { return hovering_frame == last_hover_frame; }
        bool Menu_Item_hover (Vec, Vec) override;
        bool Menu_Item_click (Vec, Vec) override;
        void Menu_Item_draw (Vec, Vec) override;
    };

    struct VBox : Button {
        std::vector<std::unique_ptr<Menu_Item>> contents;
        Vec cached_area;
        Vec cached_size;
        Vec Menu_Item_size (Vec area) override;
        void Menu_Item_draw (Vec, Vec) override;
        bool Menu_Item_hover (Vec, Vec) override;
        bool Menu_Item_click (Vec, Vec) override;

         // Type of contents confuses copy assignability testing
        VBox& operator = (const VBox&) = delete;
    };

    struct Text_Button : Button {
        std::string text;
        vis::Font* font = NULL;
        Vec cached_area;
        Vec cached_size;
        Vec Menu_Item_size (Vec area) override;
        void Menu_Item_draw (Vec, Vec) override;
        Text_Button () {
            background_color = 0x0000007f;
            hover_background_color = 0x000000ff;
        }
    };

    struct SubMenu : Text_Button {
        std::unique_ptr<Menu_Item> contents;
        bool open = false;
        void Menu_Item_draw (Vec, Vec) override;
        bool Menu_Item_hover (Vec, Vec) override;
        bool Menu_Item_click (Vec, Vec) override;
    };

}

#endif
