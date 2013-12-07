/* vim: set sw=4 ts=4 sts=4 et: */
#undef NDEBUG
#define _POSIX_C_SOURCE 200809L
#include <l4array2.h>

#include <assert.h>
#include <stdio.h>
#include <string.h>

void test_array2_op (void) {
    LbsArray2* array2 = lbs_array2_new (sizeof (int), 5, 8);
    int tc[5][8] = {
        { 1, 2, 3, 4, 5, 6, 7, 8 },
        { 9, 8, 7, 6, 5, 4, 3, 2 },
        { 0, 0, 0, 0, 0, 0, 0, 0 },
        { 5, 5, 5, 5, 5, 5, 5, 5 },
        { 1, 5, 7, 2, 8, 3, 9, 4 }
    };
    int tc_flat[5 * 8] = {
          1, 2, 3, 4, 5, 6, 7, 8,
          9, 8, 7, 6, 5, 4, 3, 2,
          0, 0, 0, 0, 0, 0, 0, 0,
          5, 5, 5, 5, 5, 5, 5, 5,
          1, 5, 7, 2, 8, 3, 9, 4
    };
    int tc_buf[5 * 8];
    const ssize_t tc_len = sizeof (int) * 5 * 8;

    lbs_array2_ref (array2);
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 8; j++) {
            lbs_array2_v (array2, int, i, j) = tc[i][j];
        }
    }

    lbs_array2_copy_out (array2, tc_buf);
    assert (memcmp (tc_buf, tc_flat, tc_len) == 0);
    lbs_array2_v (array2, int, 4, 7) = 80;
    lbs_array2_v (array2, int, 0, 0) = 90;
    lbs_array2_copy_in (array2, tc_flat);
    assert (memcmp (tc_buf, lbs_array2_get_data (array2), tc_len) == 0);

    assert (lbs_array2_get_ref_count (array2) == 2);
    lbs_array2_unref (array2);
    assert (lbs_array2_get_ref_count (array2) == 1);
    lbs_array2_unref (array2);

    printf ("%s => PASS!\n", __func__);
}

int main () {
    test_array2_op ();
    return 0;
}
