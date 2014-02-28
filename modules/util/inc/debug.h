#ifndef HAVE_UTIL_DEBUG_H
#define HAVE_UTIL_DEBUG_H

#include <stdio.h>
#include <string>
#include <unordered_map>

 // This provides a tag-based logging system.

namespace util {
    extern int logging_frame;
    void log (const char* tag, std::string msg);
    void log (const char* tag, const char* fmt, ...);
}

#endif
