#ifndef GITCGEARS_UTIL_H_INC
#define GITCGEARS_UTIL_H_INC
#include <stdio.h>
#include <string.h>

/// printing functions
#define gears_println(str, ...) (printf(str "\n", __VA_ARGS__))

#define gears_logln(out, str, ...) (fprintf(out, str "\n", __VA_ARGS__))
#define gears_wrnln(str, ...) (gears_logln(stderr, gearsio_yellow str gearsio_reset, __VA_ARGS__))
#define gears_errln(str, ...) (gears_logln(stderr, gearsio_red str gearsio_reset, __VA_ARGS__))

#define gearsio_reset "\x1b[0m"
#define gearsio_black "\x1b[30m"
#define gearsio_red "\x1b[31m"
#define gearsio_green "\x1b[32m"
#define gearsio_yellow "\x1b[33m"
#define gearsio_blue "\x1b[34m"
#define gearsio_magenta "\x1b[35m"
#define gearsio_cyan "\x1b[36m"
#define gearsio_white "\x1b[37m"

#define gearsio_bright_black gearsio_black ";1m"
#define gearsio_bright_red gearsio_red ";1m"
#define gearsio_bright_green gearsio_green ";1m"
#define gearsio_bright_yellow gearsio_yellow ";1m"
#define gearsio_bright_blue gearsio_blue ";1m"
#define gearsio_bright_magenta gearsio_magenta ";1m"
#define gearsio_bright_cyan gearsio_cyan ";1m"
#define gearsio_bright_white gearsio_white ";1m"

#define gearsio_bg_black "\x1b[40m"
#define gearsio_bg_red "\x1b[41m"
#define gearsio_bg_green "\x1b[42m"
#define gearsio_bg_yellow "\x1b[43m"
#define gearsio_bg_blue "\x1b[44m"
#define gearsio_bg_magenta "\x1b[45m"
#define gearsio_bg_cyan "\x1b[46m"
#define gearsio_bg_white "\x1b[47m"

#define gearsio_bg_bright_black gearsio_bg_black ";1m"
#define gearsio_bg_bright_red gearsio_bg_red ";1m"
#define gearsio_bg_bright_green gearsio_bg_green ";1m"
#define gearsio_bg_bright_yellow gearsio_bg_yellow ";1m"
#define gearsio_bg_bright_blue gearsio_bg_blue ";1m"
#define gearsio_bg_bright_magenta gearsio_bg_magenta ";1m"
#define gearsio_bg_bright_cyan gearsio_bg_cyan ";1m"
#define gearsio_bg_bright_white gearsio_bg_white ";1m"

/// array utils
#define ARRAY_COUNT(arr) (sizeof((arr)) / sizeof((arr)[0]))

/// string utils
#define STRING_EQUALS(a, b) (strcmp((a), (b)) == 0)

#endif // GITCGEARS_UTIL_H_INC
