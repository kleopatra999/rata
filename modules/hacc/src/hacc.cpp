

#include "../inc/hacc.h"

namespace hacc {

 // Generic internal data
struct Hacc::Contents {
    String type;
    String id;
    Valtype valtype;
    Contents (String type, String id, Valtype valtype) :
        type(type), id(id), valtype(valtype)
    { }
};

 // Specific internal data
template <class T>
struct Hacc::Value : Hacc::Contents {
    T value;
    Value (String type, String id, const T& value) :
        Contents(type, id, valtype_of<T>()), value(value)
    { }
};

 // Produce error message
const char* Error::what () const throw() {
    char ls [32]; sprintf((char*)ls, "%d", line);
    char cs [32]; sprintf((char*)cs, "%d", col);
    String r = line
        ? file.empty()
            ? mess + " at line " + (const char*)ls
                   + " col " + (const char*)cs
            : mess + " at " + file
                   + " line " + (const char*)ls
                   + " col " + (const char*)cs
        : mess;
    return r.c_str();
}


 // map C++ types to their valtypes.
template <class VTYPE> Valtype valtype_of () {
    static_assert(sizeof(VTYPE)?false:false, "The aforementioned VTYPE isn't a HACC value type.");
    return VALNULL;
}
template <> Valtype valtype_of<Null   > () { return VALNULL; }
template <> Valtype valtype_of<Bool   > () { return BOOL   ; }
template <> Valtype valtype_of<Integer> () { return INTEGER; }
template <> Valtype valtype_of<Float  > () { return FLOAT  ; }
template <> Valtype valtype_of<Double > () { return DOUBLE ; }
template <> Valtype valtype_of<String > () { return STRING ; }
template <> Valtype valtype_of<Ref    > () { return REF    ; }
template <> Valtype valtype_of<Array  > () { return ARRAY  ; }
template <> Valtype valtype_of<Object > () { return OBJECT ; }
template <> Valtype valtype_of<Error  > () { return ERROR  ; }
 // Get string name of a valtype
const char* valtype_name (Valtype t) {
    switch (t) {
        case VALNULL: return "null";
        case BOOL: return "bool";
        case INTEGER: return "integer";
        case FLOAT: return "float";
        case DOUBLE: return "double";
        case STRING: return "string";
        case REF: return "ref";
        case ARRAY: return "array";
        case OBJECT: return "object";
        case ERROR: return "error";
        default: return "corrupted";
    }
}


 // Constructors
Hacc::Hacc (      Null    n, String type, String id) : contents(new Value<Null   >(type, id, n)) { }
Hacc::Hacc (      Bool    b, String type, String id) : contents(new Value<Bool   >(type, id, b)) { }
Hacc::Hacc (      char    i, String type, String id) : contents(new Value<Integer>(type, id, i)) { }
Hacc::Hacc (      int8    i, String type, String id) : contents(new Value<Integer>(type, id, i)) { }
Hacc::Hacc (      uint8   i, String type, String id) : contents(new Value<Integer>(type, id, i)) { }
Hacc::Hacc (      int16   i, String type, String id) : contents(new Value<Integer>(type, id, i)) { }
Hacc::Hacc (      uint16  i, String type, String id) : contents(new Value<Integer>(type, id, i)) { }
Hacc::Hacc (      int32   i, String type, String id) : contents(new Value<Integer>(type, id, i)) { }
Hacc::Hacc (      uint32  i, String type, String id) : contents(new Value<Integer>(type, id, i)) { }
Hacc::Hacc (      int64   i, String type, String id) : contents(new Value<Integer>(type, id, i)) { }
Hacc::Hacc (      uint64  i, String type, String id) : contents(new Value<Integer>(type, id, i)) { }
Hacc::Hacc (      Float   f, String type, String id) : contents(new Value<Float  >(type, id, f)) { }
Hacc::Hacc (      Double  d, String type, String id) : contents(new Value<Double >(type, id, d)) { }
Hacc::Hacc (const String& s, String type, String id) : contents(new Value<String >(type, id, s)) { }
Hacc::Hacc (const Ref&    r, String type, String id) : contents(new Value<Ref    >(type, id, r)) { }
Hacc::Hacc (const Array&  a, String type, String id) : contents(new Value<Array  >(type, id, a)) { }
Hacc::Hacc (const Object& o, String type, String id) : contents(new Value<Object >(type, id, o)) { }
Hacc::Hacc (const Error&  e, String type, String id) : contents(new Value<Error  >(type, id, e)) { }

Valtype Hacc::valtype () const { return contents->valtype; }

 // getters
      Null    Hacc::assume_null    () const { return static_cast<Value<Null   >&>(*contents).value; }
      Bool    Hacc::assume_bool    () const { return static_cast<Value<Bool   >&>(*contents).value; }
      Integer Hacc::assume_integer () const { return static_cast<Value<Integer>&>(*contents).value; }
      Float   Hacc::assume_float   () const { return static_cast<Value<Float  >&>(*contents).value; }
      Double  Hacc::assume_double  () const { return static_cast<Value<Double >&>(*contents).value; }
const String& Hacc::assume_string  () const { return static_cast<Value<String >&>(*contents).value; }
const Ref&    Hacc::assume_ref     () const { return static_cast<Value<Ref    >&>(*contents).value; }
const Array&  Hacc::assume_array   () const { return static_cast<Value<Array  >&>(*contents).value; }
const Object& Hacc::assume_object  () const { return static_cast<Value<Object >&>(*contents).value; }
const Error&  Hacc::assume_error   () const { return static_cast<Value<Error  >&>(*contents).value; }
      Null    Hacc::get_null    () const { if (valtype() == VALNULL) return assume_null(); else throw valtype_error("a null"); }
      Bool    Hacc::get_bool    () const { if (valtype() == BOOL) return assume_bool(); else throw valtype_error("a bool"); }
      Integer Hacc::get_integer () const { if (valtype() == INTEGER) return assume_integer(); else throw valtype_error("an integer"); }
      Float   Hacc::get_float   () const {
    switch (valtype()) {
         // Automatic conversion
        case INTEGER: return assume_integer();
        case FLOAT:   return assume_float();
        case DOUBLE:  return assume_double();
        default: throw valtype_error("a number");
    }
}
      Double  Hacc::get_double  () const {
    switch (valtype()) {
         // Automatic conversion
        case INTEGER: return assume_integer();
        case FLOAT:   return assume_float();
        case DOUBLE:  return assume_double();
        default: throw valtype_error("a number");
    }
}
const String& Hacc::get_string  () const { if (valtype() == STRING) return assume_string(); else throw valtype_error("a string"); }
const Ref&    Hacc::get_ref     () const { if (valtype() == REF) return assume_ref(); else throw valtype_error("a ref"); }
const Array&  Hacc::get_array   () const { if (valtype() == ARRAY) return assume_array(); else throw valtype_error("an array"); }
const Object& Hacc::get_object  () const { if (valtype() == OBJECT) return assume_object(); else throw valtype_error("an object"); }
 // Phew!  So many lines for such simple concepts.


String Hacc::type () const { return contents->type; }
void Hacc::set_type (String s) { contents->type = s; }
String Hacc::id () const { return contents->id; }
void Hacc::set_id (String s) { contents->id = s; }
void Hacc::default_type_id (String type, String id) {
    if (contents->type.empty()) contents->type = type;
    if (contents->id.empty()) contents->id = id;
}

Hacc::operator Bool () const { return valtype() != ERROR; }
String Hacc::error_message () const { return *this ? "" : assume_error().what(); }

Error Hacc::valtype_error (String expected) const {
    if (valtype() == ERROR) return assume_error();
    else return Error("This Hacc is not " + expected + ", it's of valtype " + valtype_name(valtype()));
}

}

