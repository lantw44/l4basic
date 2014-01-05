/* vim: set sw=4 ts=4 sts=4 et: */
#undef NDEBUG
#define _POSIX_C_SOURCE 200809L
#define _NETBSD_SOURCE /* Get strdup(3) on NetBSD */
#include <l4array.h>

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_array_new (void) {
    const char tc[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    const size_t tc_len = sizeof (tc) / sizeof (char);

    LbsArray* array = lbs_array_new (sizeof (char));
    for (int i = 0; i < tc_len; i++) {
        lbs_array_append_data (array, &tc[i]);
    }

    assert (lbs_array_get_len (array) == tc_len);
    assert (lbs_array_get_size (array) == sizeof (char));
    assert (lbs_array_get_is_alloc (array) == true);
    assert (memcmp (tc, lbs_array_get_data (array), sizeof (tc)) == 0);
    lbs_array_unref (array);

    printf ("%s => PASS!\n", __func__);
}

void test_array_init (void) {
    const char tc[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    const size_t tc_len = sizeof (tc) / sizeof (char);

    LbsArray array_struct;
    LbsArray* array = &array_struct;
    lbs_array_init (array, sizeof (char));
    for (int i = 0; i < tc_len; i++) {
        lbs_array_append_data (array, &tc[i]);
    }

    assert (lbs_array_get_len (array) == tc_len);
    assert (lbs_array_get_size (array) == sizeof (char));
    assert (lbs_array_get_is_alloc (array) == false);
    assert (memcmp (tc, lbs_array_get_data (array), sizeof (tc)) == 0);
    lbs_array_unref (array);

    printf ("%s => PASS!\n", __func__);
}

void test_array_copy (void) {
    const char tc[] = "b1dc8070-3847-4ec6-8486-65d4e32d16b5";
    const size_t tc_len = sizeof (tc) / sizeof (char);

    LbsArray* array = lbs_array_new_with_max (sizeof (char), tc_len);
    for (int i = 0; i < tc_len; i++) {
        lbs_array_append_data (array, &tc[i]);
    }

    LbsArray* array_copy = lbs_array_copy (NULL, array);
    assert (lbs_array_get_len (array) == lbs_array_get_len (array_copy));
    assert (lbs_array_get_size (array) == lbs_array_get_size (array_copy));
    assert (memcmp (lbs_array_get_data (array),
        lbs_array_get_data (array_copy), sizeof (tc)) == 0);

    lbs_array_unref (array);
    lbs_array_unref (array_copy);

    printf ("%s => PASS!\n", __func__);
}

void test_array_cat (void) {
    char* tc1 = malloc (15);
    char* tc2 = malloc (15);
    strcpy (tc1, "!@#$%^&*()TGQ");
    strcpy (tc2, ":;<>?/[]{}");
    size_t tc1_len = strlen (tc1);
    size_t tc2_len = strlen (tc2);

    LbsArray *cat, *a1, *a2, a2_struct;
    a1 = lbs_array_make_struct (NULL, sizeof (char), tc1_len, 15, tc1);
    a2 = &a2_struct;
    a2 = lbs_array_make_struct (a2, sizeof (char), tc2_len, 15, tc2);
    cat = lbs_array_cat (a1, a2);

    assert (a1 == cat);
    assert (lbs_array_get_len (cat) == tc1_len + tc2_len);
    assert (lbs_array_get_size (cat) == lbs_array_get_size (a2));
    assert (memcmp (lbs_array_get_data (cat),
        lbs_array_get_data (a1), sizeof (char) * tc1_len) == 0);
    assert (memcmp (lbs_array_vp (cat, tc1_len),
        lbs_array_get_data (a2), sizeof (char) * tc2_len) == 0);

    lbs_array_unref (a1);
    free (lbs_array_drop_struct (a2));

    printf ("%s => PASS!\n", __func__);
}

void test_array_ref (void) {
    LbsArray* array = lbs_array_new (16);
    assert (lbs_array_get_ref_count (array) == 1);
    assert (lbs_array_ref (array) == array);
    assert (lbs_array_ref (array) == array);
    assert (lbs_array_ref (array) == array);
    assert (lbs_array_get_ref_count (array) == 4);
    lbs_array_unref (array);
    lbs_array_unref (array);
    lbs_array_unref (array);
    assert (lbs_array_get_ref_count (array) == 1);
    lbs_array_unref (array);

    printf ("%s => PASS!\n", __func__);
}

void test_array_ptr (void) {
    const char* tc[] = {
        "260740d8-6d94-41fc-9235-ee6b11209547",
        "8613ff0b-4429-4432-8870-a79477665a89",
        "b05adbb1-b628-4b49-8661-5576c477ea6e",
        "641992b6-c5a5-4855-9448-11343a60ad67",
        "74380d26-376a-4754-be5e-41cb62e52d86",
        NULL
    };
    const size_t tc_len = sizeof (tc) / sizeof (char*);

    /* This is a pointer array, so we use generic pointer (void*) */
    LbsArray* ptr_array = lbs_array_new (sizeof (void*));
    lbs_array_set_free_func (ptr_array, free);
    for (int i = 0; i < tc_len; i++) {
        if (tc[i] != NULL) {
            lbs_array_append_ptr (ptr_array, strdup (tc[i]));
        } else {
            lbs_array_append_ptr (ptr_array, NULL);
        }
    }

    assert (lbs_array_v (ptr_array, char*, tc_len - 1) == NULL);
    assert (lbs_array_get_len (ptr_array) == tc_len);
    for (int i = 0; i < tc_len - 1; i++) {
        if (tc[i] != NULL) {
            assert (strcmp (lbs_array_v (ptr_array, char*, i), tc[i]) == 0);
        }
    }
    lbs_array_unref (ptr_array);

    printf ("%s => PASS!\n", __func__);
}

void test_array_op (void) {
    LbsArray* array = lbs_array_new (sizeof (int));
    for (int i = 0; i < 80; i++) {
        lbs_array_append_var (array, i);
    }

    lbs_array_remove (array);
    lbs_array_remove (array);
    lbs_array_remove (array);
    assert (lbs_array_get_len (array) == 77);

    array->len = 20;
    lbs_array_minimize (array);
    assert (lbs_array_get_len (array) == 20);

    array->len = 10;
    lbs_array_v (array, int, 15) = 82;
    for (int i = 0; i < 10; i++) {
        assert (lbs_array_v (array, int, i) == i);
    }

    lbs_array_unref (array);
    printf ("%s => PASS!\n", __func__);
}

int main () {
    test_array_new ();
    test_array_init ();
    test_array_copy ();
    test_array_cat ();
    test_array_ref ();
    test_array_ptr ();
    test_array_op ();
    return 0;
}
