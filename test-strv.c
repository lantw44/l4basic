/* vim: set sw=4 ts=4 sts=4 et: */
#undef NDEBUG
#define _POSIX_C_SOURCE 200809L
#include <l4strv.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_strv_new (void) {
    const char* tc[] = {
        "ABC", "DEF", "GHIJKL", "MNOP", "QRST", "UVWXYZ", NULL
    };
    const size_t tc_len = sizeof (tc) / sizeof (char*);

    LbsStrv* strv = lbs_strv_new ();
    for (int i = 0; i < tc_len; i++) {
        lbs_strv_append_str (strv, tc[i]);
    }

    assert (lbs_strv_get_len (strv) == tc_len - 1);
    for (int i = 0; i < lbs_strv_get_len (strv); i++) {
        char* str = lbs_strv_dup_str (strv, i);
        assert (strcmp (str, tc[i]) == 0);
        free (str);
    }

    assert (lbs_strv_get_ref_count (strv) == 1);
    assert (lbs_strv_get_is_alloc (strv) == true);
    lbs_strv_unref (strv);

    printf ("%s => PASS!\n", __func__);
}

void test_strv_init (void) {
    const char* tc[] = {
        "ABC", "DEF", "GHIJKL", "MNOP", "QRST", "UVWXYZ", NULL
    };
    const size_t tc_len = sizeof (tc) / sizeof (char*);

    LbsStrv strv_struct;
    LbsStrv* strv = &strv_struct;
    lbs_strv_init (strv);
    for (int i = 0; i < tc_len; i++) {
        lbs_strv_append_str (strv, tc[i]);
    }

    assert (lbs_strv_get_len (strv) == tc_len - 1);
    for (int i = 0; i < lbs_strv_get_len (strv); i++) {
        char* str = lbs_strv_dup_str (strv, i);
        assert (strcmp (str, tc[i]) == 0);
        free (str);
    }

    assert (lbs_strv_get_ref_count (strv) == 1);
    assert (lbs_strv_get_is_alloc (strv) == false);
    lbs_strv_unref (strv);

    printf ("%s => PASS!\n", __func__);
}

void test_strv_copy (void) {
    const char* tc1[] = {
        "ABC", "DEF", "GHIJKL", "MNOP", "QRST", "UVWXYZ", NULL
    };
    const size_t tc1_len = sizeof (tc1) / sizeof (char*);

    const char* tc2[] = {
        "01234", "5678", "9+_()", "<>?{}[]!@#$%^&*", NULL
    };
    const size_t tc2_len = sizeof (tc2) / sizeof (char*);

    LbsStrv* strv = lbs_strv_new ();
    lbs_strv_minimize (strv);
    lbs_strv_append_strv (strv, tc1);
    lbs_strv_append_strv (strv, tc2);
    lbs_strv_minimize (strv);

    assert (lbs_strv_get_len (strv) == lbs_strv_get_max (strv));
    assert (lbs_strv_get_len (strv) == tc1_len + tc2_len - 2);

    char** copy1 = lbs_strv_copy_strv (strv);
    char** copy2 = lbs_strv_drop_struct (strv);

    int vi = 0;
    for (int i = 0; i < tc1_len - 1; i++, vi++) {
        assert (strcmp (copy1[vi], tc1[i]) == 0);
        assert (strcmp (copy2[vi], tc1[i]) == 0);
    }
    for (int i = 0; i < tc2_len - 1; i++, vi++) {
        assert (strcmp (copy1[vi], tc2[i]) == 0);
        assert (strcmp (copy2[vi], tc2[i]) == 0);
    }

    lbs_strv_generic_free (copy1);
    lbs_strv_generic_free (copy2);

    printf ("%s => PASS!\n", __func__);
}

void test_strv_ref (void) {
    LbsStrv* strv = lbs_strv_new ();
    assert (lbs_strv_get_ref_count (strv) == 1);
    assert (lbs_strv_ref (strv) == strv);
    assert (lbs_strv_get_ref_count (strv) == 2);
    lbs_strv_unref (strv);
    assert (lbs_strv_get_ref_count (strv) == 1);
    lbs_strv_unref (strv);

    printf ("%s => PASS!\n", __func__);
}

void test_strv_op (void) {
    const char* tc[] = { "ABC", "UVWXYZ", NULL };
    LbsStrv* strv = lbs_strv_new ();
    lbs_strv_append_char (strv, 0, 'a');
    lbs_strv_append_char (strv, 0, 't');
    lbs_strv_append_char (strv, 0, 's');
    lbs_strv_append_str (strv, "strv");
    lbs_strv_append_strv (strv, tc);
    lbs_strv_remove_char (strv, 3);
    lbs_strv_remove_char (strv, 3);
    lbs_strv_remove_char (strv, 3);
    lbs_strv_append_str (strv, "Good");
    lbs_strv_append_str (strv, "AC_AC_AC_AC_AC_AC_AC_AC_AC_AC_AC_AC_AC_AC");
    lbs_strv_remove_str (strv);

    assert (lbs_strv_get_len (strv) == 5);

    assert (lbs_strv_get_str_len (strv, 0) == 3);
    assert (memcmp (
        lbs_strv_get_str_not_null_terminated (strv, 0), "ats", 3) == 0);

    assert (lbs_strv_get_str_len (strv, 1) == 4);
    assert (memcmp (
        lbs_strv_get_str_not_null_terminated (strv, 1), "strv", 4) == 0);

    assert (lbs_strv_get_str_len (strv, 2) == 3);
    assert (memcmp (
        lbs_strv_get_str_not_null_terminated (strv, 2), "ABC", 3) == 0);

    assert (lbs_strv_get_str_len (strv, 3) == 3);
    assert (memcmp (
        lbs_strv_get_str_not_null_terminated (strv, 3), "UVW", 3) == 0);

    assert (lbs_strv_get_str_len (strv, 4) == 4);
    assert (memcmp (
        lbs_strv_get_str_not_null_terminated (strv, 4), "Good", 4) == 0);

    lbs_strv_unref (strv);

    printf ("%s => PASS!\n", __func__);
}

void test_strv_generic (void) {
    const char* tc[] = { "ABC", "DEF", "GHIJKL", "UVWXYZ", NULL };
    LbsStrv* strv = lbs_strv_new ();
    lbs_strv_append_strv (strv, tc);

    char** copy1 = lbs_strv_copy_strv (strv);
    char** copy2 = lbs_strv_generic_build (tc[0], tc[1], tc[2], tc[3], tc[4]);

    for (int i = 0; i < 4; i++) {
        assert (strcmp (copy1[i], tc[i]) == 0);
        assert (strcmp (copy2[i], tc[i]) == 0);
    }

    assert (lbs_strv_generic_cmp (
        LBS_STRV_GENERIC_CONST (copy1),
        LBS_STRV_GENERIC_CONST (copy2)) == 0);

    lbs_strv_unref (strv);
    lbs_strv_generic_free (copy1);
    lbs_strv_generic_free (copy2);

    printf ("%s => PASS!\n", __func__);
}

int main () {
    test_strv_new ();
    test_strv_init ();
    test_strv_copy ();
    test_strv_ref ();
    test_strv_op ();
    test_strv_generic ();
    return 0;
}
