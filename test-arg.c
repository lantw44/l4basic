/* vim: set sw=4 ts=4 sts=4 et: */
#undef NDEBUG
#define _POSIX_C_SOURCE 200809L
#include <l4arg.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_arg_parse (void) {
    LbsStrv* strv;
    LbsArray* detail;

    strv = lbs_arg_parse (
        ",''/test-arg  yes,, \"AB\\^^\\CDE\" \'^^\\\\\' YY\\ ZZ $(F GH\"I\"^)),",
        " ,", "^\\", (LbsArgQuote[]) {
            { "\"", "\"", false },
            { "\'", "\'", true },
            { "$(", ")", false },
            { NULL, NULL, false }
        }, &detail);

    assert (lbs_strv_get_len (strv) == lbs_array_get_len (detail));
    assert (lbs_strv_get_len (strv) == 6);

    char** strv_copy = lbs_strv_copy_strv (strv);
    char* strv_ans[] = {
        "/test-arg", "yes", "AB^\\CDE", "^^\\\\", "YY ZZ", "F GH\"I\")", NULL};
    for (int i = 0; strv_ans[i] != NULL; i++) {
        assert (strcmp (strv_copy[i], strv_ans[i]) == 0);
    }

    int detail_ans[] = { 1, -1, 0, 1, -1, 2 };
    for (size_t i = 0; i < lbs_array_get_len (detail); i++) {
        assert (lbs_array_v (detail, int, i) == detail_ans[i]);
    }

    lbs_strv_generic_free (strv_copy);
    lbs_strv_unref (strv);
    lbs_array_unref (detail);

    printf ("%s => PASS!\n", __func__);
}

void test_qarg_parse (void) {
    LbsArgQopt* qopt = lbs_arg_qopt_new (
        "if=virt,file=/Yes/Good\\ Night/Great,\"name=\\Y\",good");

    assert (lbs_arg_qopt_get_len (qopt) == 4);
    assert (lbs_arg_qopt_get_len (qopt) == lbs_strv_get_len (qopt->strv));
    assert (lbs_arg_qopt_get_len (qopt) == lbs_array_get_len (qopt->detail));

    assert (strcmp (lbs_arg_qopt_get_name (qopt, 0), "if") == 0);
    assert (strcmp (lbs_arg_qopt_get_value (qopt, 0), "virt") == 0);
    assert (lbs_arg_qopt_has_name (qopt, 0));
    assert (lbs_arg_qopt_has_value (qopt, 0));
    assert (!lbs_arg_qopt_is_empty (qopt, 0));

    assert (strcmp (lbs_arg_qopt_get_name (qopt, 1), "file") == 0);
    assert (strcmp (lbs_arg_qopt_get_value (qopt, 1), "/Yes/Good Night/Great") == 0);
    assert (lbs_arg_qopt_has_name (qopt, 1));
    assert (lbs_arg_qopt_has_value (qopt, 1));
    assert (!lbs_arg_qopt_is_empty (qopt, 1));

    assert (strcmp (lbs_arg_qopt_get_name (qopt, 2), "name") == 0);
    assert (strcmp (lbs_arg_qopt_get_value (qopt, 2), "Y") == 0);
    assert (lbs_arg_qopt_has_name (qopt, 2));
    assert (lbs_arg_qopt_has_value (qopt, 2));
    assert (!lbs_arg_qopt_is_empty (qopt, 2));

    assert (strcmp (lbs_arg_qopt_get_name (qopt, 3), "good") == 0);
    assert (lbs_arg_qopt_get_value (qopt, 3) == NULL);
    assert (lbs_arg_qopt_has_name (qopt, 3));
    assert (!lbs_arg_qopt_has_value (qopt, 3));
    assert (!lbs_arg_qopt_is_empty (qopt, 3));

    assert (lbs_arg_qopt_is_empty (qopt, 4));

    lbs_arg_qopt_free (qopt);

    printf ("%s => PASS!\n", __func__);
}

int main () {
    test_arg_parse ();
    test_qarg_parse ();
    return 0;
}
