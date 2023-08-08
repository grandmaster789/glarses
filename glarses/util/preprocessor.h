#ifndef GLARSES_UTIL_PREPROCESSOR_H
#define GLARSES_UTIL_PREPROCESSOR_H

// small set of macro helpers, mostly for C-style constructs
// NOTE should check with compiler compatibility; only tested with Win64/MSVC right now

#define array_size(x) (sizeof(x) / sizeof((x)[0]))

#define GLARSES_STRINGIFY(x)     #x
#define GLARSES_MAKE_STRING(x)   GLARSES_STRINGIFY(x)
#define GLARSES_CONCAT(a, b)     a##b
#define GLARSES_LINE             GLARSES_MAKE_STRING(__LINE__)
#define GLARSES_FILE_LINE(msg)   __FILE__ "(" GLARSES_LINE "): " msg
#define GLARSES_UNIQUE_SUFFIX(x) GLARSES_CONCAT(x, __COUNTER__)

#endif
