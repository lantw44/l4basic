/* vim: set sw=4 ts=4 sts=4 et: */
#ifndef LBS_STR_H
#define LBS_STR_H

#include <l4common.h>
#include <stdarg.h>

#define LBS_STR_STATIC_STRLEN(x) (sizeof(x)/sizeof(char) - 1)
#define LBS_STR_ARRAY_LEN(x,t)   (sizeof(x)/sizeof(t))

bool    lbs_str_has_prefix       (const char* str, const char* prefix);
bool    lbs_str_has_suffix       (const char* str, const char* suffix);
char*   lbs_str_printf           (const char* format, ...);
char*   lbs_str_vprintf          (const char* format, va_list ap);

#endif /* LBS_STR_H */
