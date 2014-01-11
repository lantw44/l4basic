/* vim: set sw=4 ts=4 sts=4 et: */

#include "l4str.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool lbs_str_has_suffix (const char* str, const char* suffix) {
    size_t len = strlen (str);
    size_t suflen = strlen (suffix);
    int i, j;
    for (i = len - 1, j = suflen - 1; i >= 0 && j >= 0; i--, j--) {
        if (str[i] != suffix[j]) {
            return false;
        }
    }
    if (i < 0 && j >= 0) {
        return false;
    }
    return true;
}

char* lbs_str_printf (const char* format, ...) {
    va_list ap;
    char* newstr;
    int len;

    va_start (ap, format);
    len = vsnprintf (NULL, 0, format, ap) + 1;
    va_end (ap);

    newstr = malloc (len);

    va_start (ap, format);
    vsnprintf (newstr, len, format, ap);
    va_end (ap);

    return newstr;
}

