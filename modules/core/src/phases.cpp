
#include <stdlib.h>
#include "../inc/phases.h"
#include "../inc/input.h"
#include "../inc/commands.h"
#include "../../hacc/inc/everything.h"

namespace core {

    std::vector<Phase*> all_phases;
    std::vector<Layer*> all_layers;

}

using namespace core;

HCB_BEGIN(Phase*)
    type_name("core::Phase*");
    delegate(value_functions<std::string>(
        [](Phase* const& p){ return p ? p->name : std::string("null"); },
        [](Phase*& pr, std::string name){
            for (Phase* p : all_phases)
                if (p->name == name)
                    pr = p;
            return pr = NULL;
        }
    ));
HCB_END(Phase*)

struct Allow_Command : Command {
    Phase* phase;
    void operator () () {
        if (phase)
            phase->on = true;
        else {
            print_to_console("Available phases are:\n");
            for (auto p : all_phases) {
                std::string name = p->name.empty() ? "<anonymous>" : p->name;
                print_to_console("\t" + name + " \"" + p->order + "\" " + (p->on ? "true" : "false"));
            }
        }
    }
};
HCB_BEGIN(Allow_Command)
    base<Command>("allow");
    elem(member(&Allow_Command::phase, def((Phase*)NULL)));
HCB_END(Allow_Command)

struct Disallow_Command : Command {
    Phase* phase;
    void operator () () { if (phase) phase->on = false; }
};
HCB_BEGIN(Disallow_Command)
    base<Command>("disallow");
    elem(member(&Disallow_Command::phase));
HCB_END(Disallow_Command)


HCB_BEGIN(Layer*)
    type_name("core::Layer*");
    delegate(value_functions<std::string>(
        [](Layer* const& l){ return l ? l->name : std::string("null"); },
        [](Layer*& lr, std::string name){
            for (Layer* l : all_layers)
                if (l->name == name)
                    lr = l;
            return lr = NULL;
        }
    ));
HCB_END(Layer*)

struct Show_Command : Command {
    Layer* layer;
    void operator () () {
        if (layer)
            layer->on = true;
        else {
            print_to_console("Available layers are:\n");
            for (auto l : all_layers) {
                std::string name = l->name.empty() ? "<anonymous>" : l->name;
                print_to_console("\t" + name + " \"" + l->order + "\" " + (l->on ? "true" : "false"));
            }
        }
    }
};
HCB_BEGIN(Show_Command)
    base<Command>("show");
    elem(member(&Show_Command::layer, def((Layer*)NULL)));
HCB_END(Show_Command)

struct Hide_Command : Command {
    Layer* layer;
    void operator () () { if (layer) layer->on = false; }
};
HCB_BEGIN(Hide_Command)
    base<Command>("hide");
    elem(member(&Hide_Command::layer));
HCB_END(Hide_Command)


