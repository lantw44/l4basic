/* vim: set sw=4 ts=4 sts=4 et: */

#include "l4strv.h"

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

LbsStrv* lbs_strv_new_with_max (size_t max) {
    LbsStrv* strv = malloc (sizeof (LbsStrv));
    if (strv == NULL) {
        return NULL;
    }

    if (lbs_array_init_with_max (&strv->array, sizeof (LbsArray), max) < 0) {
        free (strv);
        return NULL;
    }

    strv->ref_count = 1;
    strv->is_alloc = true;
    return strv;
}

int lbs_strv_init_with_max (LbsStrv* strv, size_t max) {
    if (lbs_array_init_with_max (&strv->array, sizeof (LbsArray), max) < 0) {
        return -1;
    }

    strv->ref_count = 1;
    strv->is_alloc = false;
    return 0;
}

void* lbs_strv_ref_generic (void* strv_generic) {
    LbsStrv* strv = LBS_STRV (strv_generic);
    strv->ref_count++;
    return strv;
}

void lbs_strv_unref_generic (void* strv_generic) {
    if (strv_generic == NULL) {
        return;
    }
    LbsStrv* strv = LBS_STRV (strv_generic);
    strv->ref_count--;
    if (strv->ref_count <= 0) {
        lbs_strv_free (strv);
    }
}

void lbs_strv_free_generic (void* strv_generic) {
    if (strv_generic == NULL) {
        return;
    }
    LbsStrv* strv = LBS_STRV (strv_generic);
    size_t i = 0;
    for (; i < lbs_strv_get_len (strv); i++) {
        LbsArray* str_wrapper = lbs_strv_get_str_wrapper (strv, i);
        lbs_array_unref (str_wrapper);
    }
    lbs_array_unref (&strv->array);
    if (strv->is_alloc) {
        free (strv);
    }
}

char* lbs_strv_dup_str (LbsStrv* strv, size_t stri) {
    LbsArray* str_wrapper = lbs_strv_get_str_wrapper (strv, stri);
    size_t len = lbs_array_get_len (str_wrapper);
    char* str = lbs_array_get_data (str_wrapper);

    char* str_new = malloc (len + sizeof (char));
    strncpy (str_new, str, len);
    str_new[len] = '\0';

    return str_new;
}

int lbs_strv_append_char (LbsStrv* strv, size_t stri, char chr) {
    if (chr == '\0') {
        return -1;
    }

    if (stri >= lbs_strv_get_len (strv)) {
        stri = lbs_strv_get_len (strv);
        LbsArray str_struct;
        LbsArray* str = &str_struct;
        if (lbs_array_init (str, sizeof (char)) < 0) {
            return -1;
        }
        if (lbs_array_append_data (&strv->array, str) < 0) {
            lbs_array_unref (str);
            return -1;
        }
    }
    LbsArray* str = lbs_strv_get_str_wrapper (strv, stri);
    return lbs_array_append_data (str, &chr);
}

int lbs_strv_append_str_empty (LbsStrv* strv) {
    LbsArray str_struct, *str = &str_struct;
    if (lbs_array_init (str, sizeof (char)) < 0) {
        return -1;
    }
    if (lbs_array_append_data (&strv->array, &str_struct) < 0) {
        lbs_array_unref (str);
        return -1;
    }
    return 0;
}

int lbs_strv_append_str (LbsStrv* strv, const char* bstr) {
    if (bstr == NULL) {
        return -1;
    }

    size_t len = strlen (bstr);
    char* str_copy = malloc (sizeof (char) * len);
    if (str_copy == NULL) {
        return -1;
    }
    strncpy (str_copy, bstr, len);

    LbsArray str_struct, *str = &str_struct;
    if (lbs_array_make_struct (str, sizeof (char), len, len, str_copy) == NULL)
    {
        free (str_copy);
        return -1;
    }

    if (lbs_array_append_data (&strv->array, &str_struct) < 0) {
        lbs_array_unref (str);
        free (str_copy);
        return -1;
    }

    return 0;
}

int lbs_strv_append_strv (LbsStrv* strv, const char* const* bstrv) {
    if (bstrv == NULL) {
        return -1;
    }

    int i;
    for (i = 0; bstrv[i] != NULL; i++) {
        if (lbs_strv_append_str (strv, bstrv[i]) < 0) {
            return -1;
        }
    }
    return 0;
}

int lbs_strv_remove_str (LbsStrv* strv) {
    size_t len = lbs_strv_get_len (strv);
    if (len <= 0) {
        return -1;
    }

    lbs_array_unref (lbs_strv_get_str_wrapper (strv, len - 1));
    return lbs_array_remove (&strv->array);
}

int lbs_strv_minimize (LbsStrv* strv) {
    if (lbs_array_minimize (&strv->array) < 0) {
        return -1;
    }

    size_t i;
    size_t len = lbs_strv_get_len (strv);
    for (i = 0; i < len; i++) {
        LbsArray* str = lbs_strv_get_str_wrapper (strv, i);
        if (lbs_array_minimize (str) < 0) {
            return -1;
        }
    }

    return 0;
}

char** lbs_strv_copy_strv (LbsStrv* strv) {
    size_t len = lbs_strv_get_len (strv);
    char** bstrv = malloc (sizeof (char*) * (len + 1));
    if (bstrv == NULL) {
        return NULL;
    }

    size_t i;
    for (i = 0; i < len; i++) {
        LbsArray* str = lbs_strv_get_str_wrapper (strv, i);
        size_t str_len = lbs_array_get_len (str);
        char* bstr = malloc (sizeof (char) * (str_len + 1));

        strncpy (bstr, lbs_array_get_data (str), str_len);
        bstr[str_len] = '\0';
        bstrv[i] = bstr;
    }
    bstrv[len] = NULL;
    return bstrv;
}

char** lbs_strv_drop_struct (LbsStrv* strv) {
    char** bstrv = lbs_strv_copy_strv (strv);
    if (bstrv == NULL) {
        return NULL;
    }

    lbs_strv_free (strv);
    return bstrv;
}

char** lbs_strv_generic_build (const char* str_first, ...) {
    LbsArray bstrv_w_struct, *bstrv_w = &bstrv_w_struct;
    va_list ap;
    char* str;
    char* str_copy;
    size_t str_len;

    if (lbs_array_init (bstrv_w, sizeof (void*)) < 0) {
        return NULL;
    }

    va_start (ap, str_first);

    str_len = strlen (str_first);
    str_copy = malloc (sizeof (char) * (str_len + 1));
    if (str_copy == NULL) {
        lbs_array_unref (bstrv_w);
        va_end (ap);
        return NULL;
    }
    strcpy (str_copy, str_first);

    if (lbs_array_append_ptr (bstrv_w, str_copy) < 0) {
        free (str_copy);
        lbs_array_unref (bstrv_w);
        va_end (ap);
        return NULL;
    }


    while ((str = va_arg (ap, char*)) != NULL) {
        str_len = strlen (str);
        str_copy = malloc (sizeof (char) * (str_len + 1));
        if (str_copy == NULL) {
            lbs_array_unref (bstrv_w);
            va_end (ap);
            return NULL;
        }
        strcpy (str_copy, str);

        if (lbs_array_append_ptr (bstrv_w, str_copy) < 0) {
            free (str_copy);
            lbs_array_unref (bstrv_w);
            va_end (ap);
            return NULL;
        }
    }
    if (lbs_array_append_ptr (bstrv_w, NULL) < 0) {
        lbs_array_unref (bstrv_w);
        va_end (ap);
        return NULL;
    }

    va_end (ap);

    return lbs_array_drop_struct (bstrv_w);
}

int lbs_strv_generic_cmp (
    const char* const* bstrv1, const char* const* bstrv2) {
    int i;
    for (i = 0; bstrv1[i] != NULL && bstrv2[i] != NULL; i++) {
        int r = strcmp (bstrv1[i], bstrv2[i]);
        if (r != 0) {
            return r;
        }
    }

    if (bstrv1[i] != NULL) {
        return 1;
    }
    if (bstrv2[i] != NULL) {
        return -1;
    }

    return 0;
}

void lbs_strv_generic_free (char** bstrv) {
    int i;
    for (i = 0; bstrv[i] != NULL; i++) {
        free (bstrv[i]);
    }
    free (bstrv);
}
