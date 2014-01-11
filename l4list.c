/* vim: set sw=4 ts=4 sts=4 et: */
#include "l4list.h"

#include <stdlib.h>
#include <string.h>

LbsListMeta* lbs_list_meta_new (void (*free_func) (void*)) {
    LbsListMeta* list = malloc (sizeof (LbsListMeta));
    if (list == NULL) {
        return NULL;
    }

    list->first = NULL;
    list->last = NULL;
    list->len = 0;
    list->free_func = free_func;

    return list;
}

void lbs_list_meta_free (LbsListMeta* list) {
    if (list == NULL) {
        return;
    }
    if (list->len <= 0) {
        free (list);
        return;
    }

    LbsList *iter, *next;
    for (iter = list->first; iter != NULL; iter = next) {
        next = iter->next;
        if (iter->free_func != NULL) {
            (*(iter->free_func)) (iter->data);
        } else if (list->free_func != NULL) {
            (*(list->free_func)) (iter->data);
        }
        free (iter);
    }
    free (list);
}

static LbsList* init_first_node (LbsListMeta* list,
    void* data, void (*free_func) (void*)) {

    /* Warning: This function assumes the list is empty! */
    LbsList* node = malloc (sizeof (LbsList));
    if (node == NULL) {
        return NULL;
    }

    if (list != NULL) {
        list->first = node;
        list->last = node;
        list->len = 1;
    }

    node->prev = NULL;
    node->next = NULL;
    node->data = data;
    node->free_func = free_func;

    return node;
}

LbsList* lbs_list_insert_prev (LbsListMeta* list, LbsList* node,
    void* data, void (*free_func) (void*)) {

    if (list != NULL && list->len <= 0) {
        return init_first_node (list, data, free_func);
    }

    LbsList* new_node = malloc (sizeof (LbsList));
    if (new_node == NULL) {
        return NULL;
    }

    if (node == NULL) {
        new_node->prev = NULL;
        new_node->next = NULL;
        new_node->data = data;
        new_node->free_func = free_func;
        return new_node;
    }

    if (list != NULL) {
        list->len++;
        if (list->first == node) {
            list->first = new_node;
        }
    }

    LbsList* old_prev = node->prev;
    node->prev = new_node;
    new_node->next = node;
    new_node->prev = old_prev;
    if (old_prev != NULL) {
        old_prev->next = new_node;
    }

    new_node->data = data;
    new_node->free_func = free_func;
    return new_node;
}

LbsList* lbs_list_insert_next (LbsListMeta* list, LbsList* node,
    void* data, void (*free_func) (void*)) {

    if (list != NULL && list->len <= 0) {
        return init_first_node (list, data, free_func);
    }

    LbsList* new_node = malloc (sizeof (LbsList));
    if (new_node == NULL) {
        return NULL;
    }

    if (node == NULL) {
        new_node->prev = NULL;
        new_node->next = NULL;
        new_node->data = data;
        new_node->free_func = free_func;
        return new_node;
    }

    if (list != NULL) {
        list->len++;
        if(list->last == node){
            list->last = new_node;
        }
    }

    LbsList* old_next = node->next;
    node->next = new_node;
    new_node->prev = node;
    new_node->next = old_next;
    if (old_next != NULL) {
        old_next->prev = new_node;
    }

    new_node->data = data;
    new_node->free_func = free_func;
    return new_node;
}

void lbs_list_remove (LbsListMeta* list, LbsList* node) {
    /* node must not be NULL! */
    if (list != NULL) {
        if (list->first == node) {
            list->first = node->next;
        }
        if (list->last == node) {
            list->last = node->prev;
        }
        if (list->len > 0) {
            list->len--;
        }
    }

    LbsList* old_next = node->next;
    LbsList* old_prev = node->prev;

    if (node->free_func != NULL) {
        (*(node->free_func)) (node->data);
    } else if (list != NULL && list->free_func != NULL) {
        (*(list->free_func)) (node->data);
    }
    free (node);

    if (old_next != NULL) {
        old_next->prev = old_prev;
    }
    if (old_prev != NULL) {
        old_prev->next = old_next;
    }
}

LbsList* lbs_list_goto (LbsList* node, int count) {
    int i;

    if (count > 0) { /* positive */
        for (i = 0; i < count; i++) {
            node = node->next;
            if (node == NULL) {
                return NULL;
            }
        }
    } else if (count < 0) { /* negative */
        count = -count;
        for (i = 0; i < count; i++) {
            node = node->prev;
            if (node == NULL) {
                return NULL;
            }
        }
    }

    return node;
}
