/* vim: set sw=4 ts=4 sts=4 et: */

#include "l4str.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool lbs_str_has_prefix (const char* str, const char* prefix) {
    int i;
    for (i = 0; str[i] != '\0' && prefix[i] != '\0'; i++) {
        if (str[i] != prefix[i]) {
            return false;
        }
    }
    if (str[i] == '\0' && prefix[i] == '\0') {
        return true;
    }
    if (str[i] == '\0') {
        return false;
    }
    return true;
}

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

    va_start (ap, format);
    newstr = lbs_str_vprintf (format, ap);
    va_end (ap);

    return newstr;
}

char* lbs_str_vprintf (const char* format, va_list ap) {
    va_list ap1, ap2;
    char* newstr;
    int len;

    va_copy (ap1, ap);
    len = vsnprintf (NULL, 0, format, ap1) + 1;
    va_end (ap1);

    newstr = malloc (len);

    va_copy (ap2, ap);
    vsnprintf (newstr, len, format, ap2);
    va_end (ap2);

    return newstr;
}
