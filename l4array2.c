/* vim: set sw=4 ts=4 sts=4 et: */
#include "l4array2.h"

#include <stdlib.h>
#include <string.h>

LbsArray2* lbs_array2_new (size_t size, int lenx, int leny) {
	if(size <= 0 || lenx <= 0 || leny <= 0){
		return NULL;
	}

	LbsArray2* array2 = malloc (
        sizeof (LbsArray2) + size * lenx * leny);
	if(array2 == NULL){
		return NULL;
	}

	array2->size = size;
	array2->lenx = lenx;
	array2->leny = leny;
    array2->ref_count = 1;
	return array2;
}

void lbs_array2_copy_in (LbsArray2* array2, const void* copy_in) {
    memcpy (array2->data, copy_in, array2->size * array2->lenx * array2->leny);
}

void lbs_array2_copy_out (LbsArray2* array2, void* copy_out) {
    memcpy (copy_out, array2->data, array2->size * array2->lenx * array2->leny);
}

void* lbs_array2_ref_generic (void* array2_generic) {
    LbsArray2* array2 = LBS_ARRAY2 (array2_generic);
    array2->ref_count++;
    return array2;
}

void lbs_array2_unref_generic (void* array2_generic) {
    LbsArray2* array2 = LBS_ARRAY2 (array2_generic);
    array2->ref_count--;
    if (array2->ref_count <= 0) {
        free (array2_generic);
    }
}
