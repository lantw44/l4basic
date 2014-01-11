/* vim: set sw=4 ts=4 sts=4 et: */
#undef NDEBUG
#define _POSIX_C_SOURCE 200809L
#define _NETBSD_SOURCE

#include <l4list.h>

#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define INT_TO_PTR(x) ((void*)((intptr_t)(x)))
#define PTR_TO_INT(x) ((int)((intptr_t)((void*)(x))))

void test_list_empty (void) {
    LbsListMeta* list = lbs_list_meta_new (NULL);
    LbsList* node = NULL;

    for (int i = 1; i <= 10; i++) {
        node = lbs_list_insert_next (list, node, INT_TO_PTR (i), NULL);
    }

    assert (lbs_list_meta_get_len (list) == 10);

    node = lbs_list_meta_goto (list, 5);
    lbs_list_remove (list, node);
    assert (lbs_list_meta_get_len (list) == 9);

    node = lbs_list_meta_get_first (list);
    for (int i = 1; i <= 10; i++) {
        if (i == 6) {
            continue;
        }
        assert (PTR_TO_INT (lbs_list_get_data (node)) == i);
        node = node->next;
    }

    lbs_list_meta_free (list);

    printf ("%s => PASS!\n", __func__);
}

void test_list_free_func (void) {
    LbsListMeta* list = lbs_list_meta_new (free);
    LbsList* node = NULL;
    LbsList* node_next;
    const char* a[] = {
        "1 Good",
        "2 Test",
        "3 Free",
        "4 Tree",
        "5 Book",
        "6 GGGGOOOOOOOODDDD",
        "7 OOOOOOOOKKKKKKKKKKKK",
        "8 QQQQQQQQAAAAAAAAAAAAAAAAAA",
        "9 SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSHHHHHHHHHHHHHH",
        "10 !!"
    };

    for (int i = 0; i < 10; i++) {
        node = lbs_list_insert_next (list, node, strdup (a[i]), NULL);
    }

    assert (lbs_list_meta_get_len (list) == 10);

    node = lbs_list_meta_goto (list, 5);
    node_next = node->next;
    lbs_list_remove (list, node);
    assert (lbs_list_meta_get_len (list) == 9);

    lbs_list_insert_prev (list, node_next, INT_TO_PTR (0), NULL);
    assert (lbs_list_meta_get_len (list) == 10);

    node = lbs_list_meta_get_first (list);
    for (int i = 0; i < 10; i++) {
        if (i == 5) {
            assert (PTR_TO_INT (lbs_list_get_data (node)) == 0);
            node = node->next;
            continue;
        }
        assert (strcmp (lbs_list_get_data (node), a[i]) == 0);
        node = node->next;
    }

    lbs_list_meta_free (list);
    printf ("%s => PASS!\n", __func__);
}

void test_list_op (void) {
    LbsListMeta* list = lbs_list_meta_new (NULL);
    LbsList* node;

    node = lbs_list_insert_next (list, NULL, INT_TO_PTR (2), NULL);
    node = lbs_list_insert_prev (list, node, INT_TO_PTR (1), NULL);
    lbs_list_push_back (list, INT_TO_PTR (2), NULL);
    lbs_list_push_front (list, INT_TO_PTR (0), NULL);

    lbs_list_insert_prev (list, lbs_list_meta_get_last (list),
        INT_TO_PTR (3), NULL);
    lbs_list_pop_back (list);
    lbs_list_pop_front (list);

    lbs_list_push_front (list, INT_TO_PTR (-1), NULL);
    node = lbs_list_meta_goto (list, 0);
    lbs_list_remove (list, node);
    lbs_list_meta_set_free_func (list, NULL);

    node = NULL;
    for (int i = 1; node != NULL; i++, node = node->next) {
        assert (PTR_TO_INT (lbs_list_get_data (node)) == i);
    }

    lbs_list_meta_free (list);
    printf ("%s => PASS!\n", __func__);
}

void test_list_op_no_meta (void) {
    LbsList* node_first;
    LbsList* node_last;
    LbsList* node;
    LbsList* tmp;

    node_last = lbs_list_insert_next (NULL, NULL, INT_TO_PTR (2), NULL);
    node_first = lbs_list_insert_prev (NULL, node_last, INT_TO_PTR (1), NULL);
    node_last = lbs_list_insert_next (NULL, node_last, INT_TO_PTR (2), NULL);
    node_first = lbs_list_insert_prev (NULL, node_first, INT_TO_PTR (0), NULL);

    lbs_list_insert_prev (NULL, node_last, INT_TO_PTR (3), NULL);
    tmp = node_last->prev;
    lbs_list_remove (NULL, node_last);
    node_last = tmp;
    tmp = node_first->next;
    lbs_list_remove (NULL, node_first);
    node_first = tmp;

    lbs_list_insert_prev (NULL, node_first, INT_TO_PTR (-1), NULL);
    node = lbs_list_goto (node_first, -1);
    lbs_list_remove (NULL, node);

    node = NULL;
    for (int i = 1; node != NULL; i++, node = node->next) {
        assert (PTR_TO_INT (lbs_list_get_data (node)) == i);
    }

    free (lbs_list_goto (node_first, 1));
    free (node_first);
    free (node_last);

    printf ("%s => PASS!\n", __func__);
}

void test_stack (void) {
    LbsStack* stk = lbs_stack_new (NULL);

    lbs_stack_push (stk, INT_TO_PTR (1), NULL);
    lbs_stack_push (stk, INT_TO_PTR (2), NULL);
    lbs_stack_push (stk, INT_TO_PTR (3), NULL);

    assert (lbs_stack_get_len (stk) == 3);
    assert (lbs_stack_get_data (stk) == INT_TO_PTR (3));

    lbs_stack_pop (stk);
    assert (lbs_stack_get_len (stk) == 2);
    assert (lbs_stack_get_data (stk) == INT_TO_PTR (2));

    lbs_stack_pop (stk);
    assert (lbs_stack_get_len (stk) == 1);
    assert (lbs_stack_get_data (stk) == INT_TO_PTR (1));

    lbs_stack_free (stk);
    printf ("%s => PASS!\n", __func__);
}

void test_queue (void) {
    LbsQueue* q = lbs_queue_new (NULL);

    lbs_queue_enqueue (q, INT_TO_PTR (1), NULL);
    lbs_queue_enqueue (q, INT_TO_PTR (2), NULL);
    lbs_queue_enqueue (q, INT_TO_PTR (3), NULL);

    assert (lbs_queue_get_len (q) == 3);
    assert (lbs_queue_get_front (q) == INT_TO_PTR (1));
    assert (lbs_queue_get_back (q) == INT_TO_PTR (3));
    assert (lbs_queue_get_data (q) == INT_TO_PTR (1));

    lbs_queue_dequeue (q);
    assert (lbs_queue_get_len (q) == 2);
    assert (lbs_queue_get_front (q) == INT_TO_PTR (2));
    assert (lbs_queue_get_back (q) == INT_TO_PTR (3));
    assert (lbs_queue_get_data (q) == INT_TO_PTR (2));

    lbs_queue_free (q);
    printf ("%s => PASS!\n", __func__);
}

int main () {
    test_list_empty ();
    test_list_free_func ();
    test_list_op ();
    test_list_op_no_meta ();
    test_stack ();
    test_queue ();
    return 0;
}
