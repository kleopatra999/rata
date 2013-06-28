#ifndef HAVE_HACC_TYPES_INTERNAL_H
#define HAVE_HACC_TYPES_INTERNAL_H

#include <typeindex>
#include <unordered_map>
#include "../inc/dynamicism.h"

namespace hacc {

     // These are in dynamicism.cpp
    std::unordered_map<std::type_index, Type>& types_by_cpptype ();
    std::unordered_map<String, Type>& types_by_name ();

    struct TypeData {
         // General
        const std::type_info* cpptype = null;
        size_t size = -1;
        void (* construct )(void*) = null;
        void (* destruct )(void*) = null;
        void (* copy_construct )(void*, void*) = null;
        String name;
         // Hacc-specific
        GetSet0* keys = null;
        Func<Reference (void*, String)> attrs_f;
        std::vector<std::pair<String, GetSet0*>> attr_list;
        GetSet0* length = null;
        Func<Reference (void*, size_t)> elems_f;
        std::vector<GetSet0*> elem_list;
        Func<Tree* (void*)> to_tree;
        Func<void (void*, Tree*)> prepare;
        Func<void (void*, Tree*)> fill;
        Func<void (void*, Tree*)> finish;
        GetSet0* delegate = null;
        Func<Type (void*)> pointer;

        TypeData (
            const std::type_info& cpptype,
            size_t size,
            void (* construct )(void*),
            void (* destruct )(void*),
            void (* copy_construct )(void*, void*)
        ) :
            cpptype(&cpptype), size(size), construct(construct), destruct(destruct), copy_construct(copy_construct)
        {
            types_by_cpptype().emplace(cpptype, this);
        }
    };

}

#endif
