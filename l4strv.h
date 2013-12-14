/* vim: set sw=4 ts=4 sts=4 et: */
#ifndef LBS_STRV_H
#define LBS_STRV_H

#include <l4common.h>
#include <l4array.h>

typedef struct LbsStrvStruct {
    /*< private >*/
    LbsArray array;     /* data */
    unsigned ref_count; /* reference count */
    bool is_alloc;      /* is allocated using malloc */
} LbsStrv;

#define   LBS_STRV(x)               ((LbsStrv*)(x))
#define   LBS_STRV_GENERIC(x)       ((char**)(x))
#define   LBS_STRV_GENERIC_CONST(x) ((const char* const*)(x))

#define   lbs_strv_new()            (lbs_strv_new_with_max (0))
LbsStrv*  lbs_strv_new_with_max     (size_t max);

#define   lbs_strv_init(strv)       (lbs_strv_init_with_max (strv, 0))
int       lbs_strv_init_with_max    (LbsStrv* strv, size_t max);

#define   lbs_strv_ref(strv) \
    (lbs_strv_ref_generic (LBS_COMMON_CHECK_TYPE ((strv), LbsStrv*)))
#define   lbs_strv_unref(strv) \
    (lbs_strv_unref_generic (LBS_COMMON_CHECK_TYPE ((strv), LbsStrv*)))
void*     lbs_strv_ref_generic      (void* strv);
void      lbs_strv_unref_generic    (void* strv);

#define   lbs_strv_free(array) \
    (lbs_strv_free_generic (LBS_COMMON_CHECK_TYPE ((strv), LbsStrv*)))
void      lbs_strv_free_generic     (void* strv);

#define   lbs_strv_get_len(strv) \
    (lbs_array_get_len (&(LBS_COMMON_CHECK_TYPE ((strv), LbsStrv*)->array)))
#define   lbs_strv_get_max(strv) \
    (lbs_array_get_max (&(LBS_COMMON_CHECK_TYPE ((strv), LbsStrv*)->array)))
#define   lbs_strv_get_ref_count(strv) \
    (LBS_COMMON_CHECK_TYPE ((strv), LbsStrv*)->ref_count)
#define   lbs_strv_get_is_alloc(strv) \
    (LBS_COMMON_CHECK_TYPE ((strv), LbsStrv*)->is_alloc)
#define   lbs_strv_set_len(strv,len) \
    (lbs_array_set_len (LBS_COMMON_CHECK_TYPE ((strv), LbsStrv*)->array, (len)))
#define   lbs_strv_set_max(strv,max) \
    (lbs_array_set_max (LBS_COMMON_CHECK_TYPE ((strv), LbsStrv*)->array, (max)))

char*     lbs_strv_dup_str          (LbsStrv* strv, size_t stri);
#define   lbs_strv_get_str_wrapper(strv,stri) \
    (&(lbs_array_v (&((strv)->array), LbsArray, (stri))))
#define   lbs_strv_get_str_len(strv,stri) \
    (lbs_array_get_len (lbs_strv_get_str_wrapper ((strv), (stri))))
#define   lbs_strv_get_str_not_null_terminated(strv, stri) \
    ((char*)(lbs_array_get_data (lbs_strv_get_str_wrapper ((strv), (stri)))))
#define   lbs_strv_char(strv,stri,chri) \
    (lbs_array_v (lbs_strv_get_str_wrapper (strv, stri), char, (chri)))

int       lbs_strv_append_char      (LbsStrv* strv, size_t stri, char chr);
int       lbs_strv_append_str       (LbsStrv* strv, const char* bstr);
int       lbs_strv_append_strv      (LbsStrv* strv, const char* const* bstrv);
int       lbs_strv_remove_str       (LbsStrv* strv);
#define   lbs_strv_remove_char(strv,stri) \
    (lbs_array_remove (lbs_strv_get_str_wrapper ((strv), (stri))))

int       lbs_strv_minimize         (LbsStrv* strv);
char**    lbs_strv_copy_strv        (LbsStrv* strv);
char**    lbs_strv_drop_struct      (LbsStrv* strv);

char**    lbs_strv_generic_build    (const char* str, ...);
int       lbs_strv_generic_cmp      (const char* const* bstrv1,
                                     const char* const* bstrv2);
void      lbs_strv_generic_free     (char** bstrv);

#endif /* LBS_STRV_H */
