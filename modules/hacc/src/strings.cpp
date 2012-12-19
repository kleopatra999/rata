
#include "../inc/strings.h"
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#include <string.h>


namespace hacc {

String escape_string (const String& unesc) {
    String r = "";
    for (auto p = unesc.begin(); p != unesc.end(); p++) {
        switch (*p) {
            case '"': r += "\\\""; break;
            case '\\': r += "\\\\"; break;
            case '\b': r += "\\b"; break;
            case '\f': r += "\\f"; break;
            case '\n': r += "\\n"; break;
            case '\r': r += "\\r"; break;
            case '\t': r += "\\t"; break;
            default: r += String(1, *p); break;
        }
    }
    return r;
}

String escape_ident (const String& unesc) {
    if (unesc.empty()) return "\"\"";
    for (auto p = unesc.begin(); p != unesc.end(); p++) {
        if (!isalnum(*p) && *p != '_' && *p != '-')
            return "\"" + escape_string(unesc) + "\"";
    }
    return unesc;
}


String hacc_value_to_string (const Value& v) {
    switch (v.form) {
        case NULLFORM: return "null";
        case BOOL: return v.b ? "true" : "false";
        case INTEGER: {
            char r[32];
            sprintf(r, "%" PRIi64, v.i);
            return r;
        }
        case FLOAT: {
            char r[32];
            sprintf(r, "%g~%08x", v.f, *(uint32*)&v.f);
            return r;
        }
        case DOUBLE: {
            char r [64];
            sprintf(r, "%lg~%016" PRIx64, v.d, *(uint64*)&v.d);
            return r;
        }
        case STRING: return "\"" + escape_string(v.s) + "\"";
        case REF: {
            return (v.r.type.empty() ? "&" : "&#" + escape_ident(v.r.type))
                 + "@" + escape_ident(v.r.id);
        }
        case ARRAY: {
            String r = "[";
            for (auto i = v.a.begin(); i != v.a.end(); i++) {
                r += hacc_to_string(**i);
                if (i + 1 != v.a.end()) r += ", ";
            }
            return r + "]";
        }
        case OBJECT: {
            String r = "{";
            auto nexti = v.o.begin();
            for (auto i = nexti; i != v.o.end(); i = nexti) {
                r += escape_ident(i->first);
                r += ": ";
                r += hacc_to_string(*i->second);
                nexti++;
                if (nexti != v.o.end()) r += ", ";
            }
            return r + "}";
        }
        case ERROR: throw v.e;
        default: throw Error("Corrupted Hacc tree\n");
    }
}
String hacc_to_string (const Hacc& h) {
    String r = hacc_value_to_string(h.value);
    bool show_type = h.flags & ADVERTISE_TYPE;
    bool show_id = h.flags & ADVERTISE_ID;
    if ((show_type || show_id)
     && h.value.form != ARRAY && h.value.form != OBJECT)
        r = "(" + r + ")";
    if (show_id) r = "@" + escape_ident(h.id) + r;
    if (show_type) r = "#" + escape_ident(h.type) + r;
    return r;
}
String string_from_hacc (const Hacc& h) {
    return hacc_to_string(h);
}


 // Parsing is simple enough that we don't need a separate lexer step
struct Parser {
    const char* file;
    const char* begin;
    const char* p;
    const char* end;
    Parser (String s) : file(""), begin(s.data()), p(s.data()), end(s.data()+s.length()) { }
    Parser (const char* s) : file(""), begin(s), p(s), end(s + strlen(s)) { }

    int look () { return p == end ? EOF : *p; }

     // This allows us to safely use scanf, which is so much more
     // convenient than parsing numbers by hand.  Speed is not a
     // big priority here.
    const char* safebuf_begin = NULL;
    char safebuf_array [32];
    char* safebuf () {
        if (safebuf_begin != p) {
            safebuf_begin = p;
            uint len = end - p > 31 ? 31 : end - p;
            memcpy(safebuf_array, p, len);
            safebuf_array[len] = 0;
        }
        return safebuf_array;
    }

    Error error (String s) {
         // Diagnose line and column number
         // I'm not sure the col is exactly right
        uint line = 1;
        const char* nl = begin - 1;
        for (const char* p2 = begin; p2 != p; p2++) {
            if (*p2 == '\n') {
                line++;
                nl = p2;
            }
        }
        uint col = p - nl;
        return Error(s, file, line, col);
    }

     // The following are subparsers.
     // Most parsers assume the first character is already correct.
     // If a parse fails, just throw an exception, because we've got
     // datatypes with proper destructors now.

     // Utility parsers for multiple situations.

    void parse_ws () {
        while (isspace(look())) p++;
        if (p[0] == '/' && p+1 != end && p[1] == '/') {
            while (look() != '\n' && look() != EOF) p++;
            parse_ws();
        }
    }
    String parse_stringly () {
        p++;  // for the "
        String r = "";
        bool escaped = false;
        for (;;) {
            if (escaped) {
                switch (look()) {
                    case EOF: throw error("String not terminated by end of input");
                    case '"': r += '"'; break;
                    case '\\': r += "\\"; break;
                    case '/': r += "/"; break;
                    case 'b': r += "\b"; break;
                    case 'f': r += "\f"; break;
                    case 'n': r += "\n"; break;
                    case 'r': r += "\r"; break;
                    case 't': r += "\t"; break;
                    default: throw error("Unrecognized escape sequence \\" + String(p, 1));
                }
                escaped = false;
            }
            else {
                switch (look()) {
                    case EOF: throw error("String not terminated by end of input");
                    case '"': p++; return r;
                    case '\\': escaped = true; break;
                    default: r += String(p, 1);
                }
            }
            p++;
        }
    }
    String parse_ident (String what) {
        switch (look()) {
            case EOF: throw error("Expected " + what + ", but ran into the end of input");
            case '"': return parse_stringly();
            default: {
                const char* start = p;
                while (isalnum(look()) || look() == '_' || look() == '-') p++;
                if (p == start) throw error("Expected " + what + ", but saw " + String(1, look()));
                return String(start, p - start);
            }
        }
    }
    String parse_type () {
        p++;  // For the #
        return parse_ident("a type after #");
    }
    String parse_id () {
        p++;  // For the @
        return parse_ident("an id after @");
    }

     // Parsing of specific valtypes.
     // This one could return an int, float, or double.
    Hacc parse_numeric () {
        int64 val;
        uint len;
        if (!sscanf(safebuf(), "%" SCNi64 "%n", &val, &len))
            throw error("Weird number");
        p += len;
        switch (look()) {
            case '~': return parse_bitrep();
            case '.':
            case 'e':
            case 'E':
            case 'p':  // backtrack!
            case 'P': p -= len; return parse_floating();
            default: return Hacc(val);
        }
    }
    Hacc parse_floating () {
        double val;
        uint len;
        if (!sscanf(safebuf(), "%lg%n", &val, &len))
            throw error("Weird number");
        p += len;
        switch (look()) {
            case '~': return parse_bitrep();
            default: return Hacc(val);
        }
    }
    Hacc parse_bitrep () {
        p++;  // for the ~
        uint64 rep;
        uint len;
        if (!sscanf(safebuf(), "%" SCNx64 "%n", &rep, &len))
            throw error("Missing precise bitrep after ~");
        p += len;
        switch (len) {
            case  8: return std::move(Hacc(*(float*)&rep));
            case 16: return std::move(Hacc(*(double*)&rep));
            default: throw error("Precise bitrep doesn't have 8 or 16 digits");
        }
    }
    Hacc parse_string () {
        return Hacc(parse_stringly());
    }
    Hacc parse_ref () {
        String type;
        String id;
        p++;  // For the &
        if (look() == '#') type = parse_type();
        if (look() == '@') id = parse_id();
        else throw error("Ref is missing an @id");
        return Hacc(Ref{type, id});
    }
    Hacc parse_array () {
        Array a;
        p++;  // for the [
        for (;;) {
            parse_ws();
            switch (look()) {
                case EOF: throw error("Array not terminated");
                case ':': throw error("Cannot have : in an array");
                case ',': p++; break;
                case ']': p++; return Hacc(std::move(a));
                default: a.push_back(std::unique_ptr<Hacc>(new Hacc(parse_thing()))); break;
            }
        }
    }
    Hacc parse_object () {
        Object o;
        p++;  // for the left brace
        String key;
        for (;;) {
          find_key:
            parse_ws();
            switch (look()) {
                case EOF: throw error("Object not terminated");
                case ':': throw error("Missing name before : in object");
                case ',': p++; goto find_key;
                case '}': p++; return Hacc(std::move(o));
                default: key = parse_ident("an attribute name or the end of the object"); break;
            }
            parse_ws();
            if (look() == ':') p++;
            else throw error("Missing : after name");
            parse_ws();
            switch (look()) {
                case EOF: throw error("Object not terminated");
                case ':': throw error("Extra : in object");
                case ',': throw error("Misplaced comma after : in object");
                case '}': throw error("Missing value after : in object");
                default: o.push_back(Pair<std::unique_ptr<Hacc>>(key, std::unique_ptr<Hacc>(new Hacc(parse_thing())))); break;
            }
        }
    }
    Hacc parse_parens () {
        p++;  // for the (
        Hacc&& r = parse_thing();
        parse_ws();
        if (look() == ')') p++;
        else throw error("Extra stuff in parens");
        return std::move(r);
    }

    Hacc add_prefix (Hacc&& r, String type, String id) {
        if (!type.empty()) {
            if (!r.type.empty())
                throw error("Too many #types");
            r.type = type;
            r.flags |= ADVERTISE_TYPE;
        }
        if (!id.empty()) {
            if (!r.id.empty())
                throw error("Too many @ids");
            r.id = id;
            r.flags |= ADVERTISE_ID;
        }
        return std::move(r);
    }

    Hacc parse_thing () {
        parse_ws();
        String type = "";
        String id = "";
        for (;;) switch (look()) {
            case '+':
            case '-':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9': return add_prefix(parse_numeric(), type, id);
            case '~': return add_prefix(parse_bitrep(), type, id);
            case '"': return add_prefix(parse_string(), type, id);
            case '[': return add_prefix(parse_array(), type, id);
            case '{': return add_prefix(parse_object(), type, id);
            case '(': return add_prefix(parse_parens(), type, id);
            case '&': return add_prefix(parse_ref(), type, id);
            case '#':
                if (type.empty()) type = parse_type();
                else throw error("Too many #classes");
                break;
            case '@':
                if (id.empty()) id = parse_id();
                else throw error("Too many @ids");
                break;
            default: {
                if (end - p >= 4 && 0==strncmp(p, "null", 4)) {
                    p += 4;
                    return add_prefix(Hacc(null), type, id);
                }
                if (end - p >= 4 && 0==strncmp(p, "true", 4)) {
                    p += 4;
                    return add_prefix(Hacc(true), type, id);
                }
                if (end - p >= 5 && 0==strncmp(p, "false", 5)) {
                    p += 5;
                    return add_prefix(Hacc(false), type, id);
                }
                else throw error("Unknown sequence");
            }
        }
    }
    Hacc parse_all () {
        Hacc r = parse_thing();
        parse_ws();
        if (look() == EOF) return r;
        else throw error("Extra stuff at end of document");
    }
    Hacc parse () {
        try { return parse_all(); }
        catch (Error e) { return Hacc(e); }
    }
};

 // Finally:
Hacc hacc_from_string (const String& s) { return Parser(s).parse(); }
Hacc hacc_from_string (const char* s) { return Parser(s).parse(); }
Hacc string_to_hacc (const String& s) { return hacc_from_string(s); }
Hacc string_to_hacc (const char* s) { return hacc_from_string(s); }

}


