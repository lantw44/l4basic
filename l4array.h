/* vim: set sw=4 ts=4 sts=4 et: */
#ifndef LBS_ARRAY_H
#define LBS_ARRAY_H

#include <l4common.h>

typedef struct LbsArrayStruct {
    /*< public >*/
    void* data;     /* data */
    size_t len;     /* current length */
    void (*free_func) (void* data);
                    /* function to free the element */

    /*< private >*/
    size_t size;        /* element size */
    size_t max;         /* maximal length */
    unsigned ref_count; /* reference count */
    bool is_alloc;      /* is allocated using malloc */
} LbsArray;

#define   LBS_ARRAY(x)            ((LbsArray*)(x))

#define   lbs_array_new(size) \
    (lbs_array_new_with_max (size, 0))
LbsArray* lbs_array_new_with_max  (size_t size, size_t max);

#define   lbs_array_init(array, size) \
    (lbs_array_init_with_max (array, size, 0))
int       lbs_array_init_with_max (LbsArray* array, size_t size, size_t max);

LbsArray* lbs_array_copy          (LbsArray* dest, const LbsArray* src);
LbsArray* lbs_array_cat           (LbsArray* dest, const LbsArray* more);

#define   lbs_array_ref(array) \
    (lbs_array_ref_generic (LBS_COMMON_CHECK_TYPE ((array), LbsArray*)))
#define   lbs_array_unref(array) \
    (lbs_array_unref_generic (LBS_COMMON_CHECK_TYPE ((array), LbsArray*)))
void*     lbs_array_ref_generic   (void* array);
void      lbs_array_unref_generic (void* array);

#define   lbs_array_free(array) \
    (lbs_array_free_generic (LBS_COMMON_CHECK_TYPE ((array), LbsArray*)))
void      lbs_array_free_generic  (void* array);
void*     lbs_array_drop_struct   (LbsArray* array);
LbsArray* lbs_array_make_struct   (LbsArray* array, size_t size, size_t len,
                                   size_t max, void* data);

#define   lbs_array_get_data(array) \
    (LBS_COMMON_CHECK_TYPE ((array), LbsArray*)->data)
#define   lbs_array_get_size(array) \
    (LBS_COMMON_CHECK_TYPE ((array), LbsArray*)->size)
#define   lbs_array_get_len(array) \
    (LBS_COMMON_CHECK_TYPE ((array), LbsArray*)->len)
#define   lbs_array_get_max(array) \
    (LBS_COMMON_CHECK_TYPE ((array), LbsArray*)->max)
#define   lbs_array_get_ref_count(array) \
    (LBS_COMMON_CHECK_TYPE ((array), LbsArray*)->ref_count)
#define   lbs_array_get_is_alloc(array) \
    (LBS_COMMON_CHECK_TYPE ((array), LbsArray*)->is_alloc)
#define   lbs_array_get_free_func(array) \
    (LBS_COMMON_CHECK_TYPE ((array), LbsArray*)->free_func)

int       lbs_array_set_len       (LbsArray* array, size_t len);
int       lbs_array_set_max       (LbsArray* array, size_t max);
#define   lbs_array_set_free_func(array,value) \
    ((LBS_COMMON_CHECK_TYPE ((array), LbsArray*)->free_func) = (value))

#define   lbs_array_append_var(array,var) \
    (lbs_array_append_data ((array), (&(var))))
int       lbs_array_append_ptr    (LbsArray* array, const void* ptr);
int       lbs_array_append_data   (LbsArray* array, const void* data);
int       lbs_array_remove        (LbsArray* array);
int       lbs_array_minimize      (LbsArray* array);
#define   lbs_array_push_back     lbs_array_append_data
#define   lbs_array_push          lbs_array_append_data
#define   lbs_array_pop_back      lbs_array_remove
#define   lbs_array_pop           lbs_array_remove

#define   lbs_array_vp(array, index) \
    ((void*)(((char*)((array)->data))+(((array)->size)*(index))))
#define   lbs_array_v(array, type, index) \
    (*(((type*)((array)->data))+(index)))
#define   lbs_array_index         lbs_array_v
#define   lbs_array_index_ptr     lbs_array_vp

#endif /* LBS_ARRAY_H */
