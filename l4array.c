/* vim: set sw=4 ts=4 sts=4 et: */
#include "l4array.h"

#include <stdlib.h>
#include <string.h>

LbsArray* lbs_array_new_with_max (size_t size, size_t max) {
    if (size <= 0) {
        return NULL;
    }

    LbsArray* array = malloc (sizeof (LbsArray));
    if (array == NULL) {
        return NULL;
    }

    if (max > 0) {
        void* data = malloc (size * max);
        if (data == NULL) {
            free (array);
            return NULL;
        }
        array->data = data;
    } else {
        array->data = NULL;
    }

    array->free_func = NULL;
    array->size = size;
    array->len = max;
    array->max = max;
    array->ref_count = 1;
    array->is_alloc = true;

    return array;
}

int lbs_array_init_with_max (LbsArray* array, size_t size, size_t max) {
    if (size <= 0) {
        return -1;
    }

    if (max > 0) {
        void* data = malloc (size * max);
        if (data == NULL) {
            return -1;
        }
        array->data = data;
    } else {
        array->data = NULL;
    }

    array->free_func = NULL;
    array->size = size;
    array->len = max;
    array->max = max;
    array->ref_count = 1;
    array->is_alloc = false;

    return 0;
}

LbsArray* lbs_array_copy (LbsArray* dest, const LbsArray* src) {
    if (dest == NULL) {
        dest = lbs_array_new_with_max (src->size, src->max);
        if (dest == NULL) {
            return NULL;
        }
    } else {
        if (lbs_array_init_with_max (dest, src->size, src->max) < 0) {
            return NULL;
        }
    }

    dest->len = src->len;
    dest->free_func = src->free_func;
    if (src->data != NULL) {
        memcpy (dest->data, src->data, src->size * src->len);
    }
    return dest;
}

LbsArray* lbs_array_cat (LbsArray* dest, const LbsArray* more) {
    if (dest == NULL) {
        return lbs_array_copy (dest, more);
    }

    if (dest->size != more->size) {
        return NULL;
    }

    int oldlen = dest->len;
    if (lbs_array_set_len (dest, dest->len + more->len) < 0) {
        return NULL;
    }

    memcpy (lbs_array_vp (dest, oldlen), more->data, more->size * more->len);
    return dest;
}

void* lbs_array_ref_generic (void* array_generic) {
    LbsArray* array = LBS_ARRAY (array_generic);
    array->ref_count++;
    return array;
}

void lbs_array_unref_generic (void* array_generic) {
    if (array_generic == NULL) {
        return;
    }
    LbsArray* array = LBS_ARRAY (array_generic);
    array->ref_count--;
    if (array->ref_count <= 0) {
        lbs_array_free (array);
    }
}

void lbs_array_free_generic (void* array_generic) {
    if (array_generic == NULL) {
        return;
    }
    LbsArray* array = LBS_ARRAY (array_generic);
    if (array->free_func != NULL) {
        int i = 0;
        char* d = array->data;
        for (; i < array->len; i++, d += array->size) {
            (*(array->free_func)) (*((void**)d));
        }
    }
    free (array->data);
    if (array->is_alloc) {
        free (array);
    }
}

void* lbs_array_drop_struct (LbsArray* array) {
    if (!array->is_alloc) {
        return array->data;
    }

    void* data = array->data;
    free (array);
    return data;
}

LbsArray* lbs_array_make_struct (LbsArray* array,
    size_t size, size_t len, size_t max, void* data) {

    if (array == NULL) {
        array = lbs_array_new (size);
        if (array == NULL) {
            return NULL;
        }
    } else {
        if (lbs_array_init (array, size) < 0) {
            return NULL;
        }
    }

    array->len = len;
    array->max = max;
    array->data = data;
    return array;
}

int lbs_array_set_len (LbsArray* array, size_t len) {
    if (len > (array->max)){
        if (lbs_array_set_max (array, len) < 0) {
            return -1;
        } else {
            array->len = len;
        }
    } else {
        array->len = len;
        return 0;
    }
    return 0;
}

int lbs_array_set_max (LbsArray* array, size_t max) {
    void* ptr = realloc (array->data, array->size * max);
    if (ptr == NULL) {
        return -1;
    }

    array->max = max;
    array->data = ptr;
    return 0;
}

int lbs_array_append_ptr (LbsArray* array, const void* ptr) {
    return lbs_array_append_data (array, &ptr);
}

int lbs_array_append_data (LbsArray* array, const void* data) {
    if (array->max < array->len + 1) {
        if (array->max > 0){
            if (lbs_array_set_max (array, array->max * 2) < 0) {
                return -1;
            }
        } else {
            if (lbs_array_set_max (array, 1) < 0){
                return -1;
            }
        }
    }

    memcpy (lbs_array_vp (array, array->len), data, array->size);
    array->len++;
    return 0;
}

int lbs_array_remove (LbsArray* array) {
    if (array->len <= 0) {
        return -1;
    }

    array->len--;
    if (array->len < array->max * 2) {
        lbs_array_minimize (array);
    }
    return 0;
}

int lbs_array_minimize (LbsArray* array) {
    if (array->max > array->len) {
        void* ptr = realloc (array->data, array->size * array->len);
        if (ptr == NULL) {
            return -1;
        }
        array->max = array->len;
        array->data = ptr;
    }
    return 0;
}
