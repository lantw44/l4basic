/* vim: set sw=4 ts=4 sts=4 et: */
#ifndef LBS_ARRAY2_H
#define LBS_ARRAY2_H

#include <l4common.h>

typedef struct LbsArray2Struct {
    /*< private >*/
    size_t size;        /* element size */
    size_t lenx;        /* x length */
    size_t leny;        /* y length */
    unsigned ref_count; /* reference count */

    /*< public >*/
    char data[];        /* data */
} LbsArray2;

#define    LBS_ARRAY2(x)            ((LbsArray2*)(x))

LbsArray2* lbs_array2_new           (size_t size, int lenx, int leny);
void       lbs_array2_copy_in       (LbsArray2* array2, const void* copy_in);
void       lbs_array2_copy_out      (LbsArray2* array2, void* copy_out);

#define   lbs_array2_ref(array2) \
    (lbs_array2_ref_generic (LBS_COMMON_CHECK_TYPE ((array2), LbsArray2*)))
#define   lbs_array2_unref(array2) \
    (lbs_array2_unref_generic (LBS_COMMON_CHECK_TYPE ((array2), LbsArray2*)))
void*      lbs_array2_ref_generic   (void* array2);
void       lbs_array2_unref_generic (void* array2);

#define   lbs_array2_get_data(array2) \
    (LBS_COMMON_CHECK_TYPE ((array2), LbsArray2*)->data)
#define   lbs_array2_get_size(array2) \
    (LBS_COMMON_CHECK_TYPE ((array2), LbsArray2*)->size)
#define   lbs_array2_get_lenx(array2) \
    (LBS_COMMON_CHECK_TYPE ((array2), LbsArray2*)->lenx)
#define   lbs_array2_get_leny(array2) \
    (LBS_COMMON_CHECK_TYPE ((array2), LbsArray2*)->leny)
#define   lbs_array2_get_ref_count(array2) \
    (LBS_COMMON_CHECK_TYPE ((array2), LbsArray2*)->ref_count)

#define   lbs_array2_v(array2,type,x,y) \
    (*(((type*)((array2)->data)) + \
    ((x)*((array2)->leny))+(y)))
#define   lbs_array2_vp(array2,x,y) \
    ((void*)(((char*)((array2)->data)) + \
    ((array2)->size)*((x)*((array2)->leny)+(y))))

#endif /* LBS_ARRAY2_H */
