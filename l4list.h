/* vim: set sw=4 ts=4 sts=4 et: */
#ifndef LBS_LIST_H
#define LBS_LIST_H

#include <l4common.h>

typedef struct LbsListStruct { /* list node */
    /*< private >*/
    struct LbsListStruct* prev;     /* pointer to the previous node */
    struct LbsListStruct* next;     /* pointer to the next node */

    /*< public >*/
    void (*free_func) (void* data); /* function to free the data pointer */
    void* data;                     /* data pointer */
} LbsList;

typedef struct LbsListMetaStruct { /* list wrapper */
    /*< private >*/
    struct LbsListStruct* first;    /* pointer to the first node in the list */
    struct LbsListStruct* last;     /* pointer to the last node in the list */
    size_t len;                     /* current length of the list */

    /*< public >*/
    void (*free_func) (void* data);
    /* Function to free the data pointer. This function is used only if
     * the free_func field in the node is set to NULL */
} LbsListMeta;

#define      LBS_LIST(x)               ((LbsList*)(x))
#define      LBS_LIST_META(x)          ((LbsListMeta*)(x))

LbsListMeta* lbs_list_meta_new         (void (*free_func) (void*));
void         lbs_list_meta_free        (LbsListMeta* list);

#define      lbs_list_meta_get_first(list) \
    (LBS_COMMON_CHECK_TYPE ((list), LbsListMeta*)->first)
#define      lbs_list_meta_get_last(list) \
    (LBS_COMMON_CHECK_TYPE ((list), LbsListMeta*)->last)
#define      lbs_list_meta_get_len(list) \
    (LBS_COMMON_CHECK_TYPE ((list), LbsListMeta*)->len)
#define      lbs_list_meta_get_free_func(list) \
    (LBS_COMMON_CHECK_TYPE ((list), LbsListMeta*)->free_func)

#define      lbs_list_meta_set_free_func(list,value) \
    ((LBS_COMMON_CHECK_TYPE ((list), LbsListMeta*)->free_func) = (value))

#define      lbs_list_get_prev(node) \
    (LBS_COMMON_CHECK_TYPE ((node), LbsList*)->prev)
#define      lbs_list_get_next(node) \
    (LBS_COMMON_CHECK_TYPE ((node), LbsList*)->next)
#define      lbs_list_get_data(node) \
    (LBS_COMMON_CHECK_TYPE ((node), LbsList*)->data)
#define      lbs_list_get_free_func(node) \
    (LBS_COMMON_CHECK_TYPE ((node), LbsList*)->free_func)

#define      lbs_list_set_data(node,value) \
    ((LBS_COMMON_CHECK_TYPE ((node), LbsList*)->data) = (value))
#define      lbs_list_set_free_func(node,value) \
    ((LBS_COMMON_CHECK_TYPE ((node), LbsList*)->free_func) = (value))

LbsList*     lbs_list_insert_prev      (LbsListMeta* list, LbsList* node,
                                        void* data, void (*free_func) (void*));
LbsList*     lbs_list_insert_next      (LbsListMeta* list, LbsList* node,
                                        void* data, void (*free_func) (void*));
void         lbs_list_remove           (LbsListMeta* list, LbsList* node);

#define      lbs_list_push_back(list,data,f) \
    (lbs_list_insert_next ((list), (lbs_list_meta_get_last (list)), (data), (f)))
#define      lbs_list_push_front(list,data,f) \
    (lbs_list_insert_prev ((list), (lbs_list_meta_get_first (list)), (data), (f)))
#define      lbs_list_pop_back(list) \
    (lbs_list_remove ((list), (lbs_list_meta_get_last ((list)))))
#define      lbs_list_pop_front(list) \
    (lbs_list_remove ((list), (lbs_list_meta_get_first ((list)))))

LbsList*     lbs_list_goto             (LbsList* node, int count);
#define      lbs_list_meta_goto(list,count) \
    (lbs_list_goto ((lbs_list_meta_get_first (list)), (count)))


#ifndef LBS_LIST_DISABLE_STACK

typedef      LbsListMeta LbsStack;
#define      lbs_stack_new(f)          (lbs_list_meta_new (f))
#define      lbs_stack_free(stack)     (lbs_list_meta_free (stack))
#define      lbs_stack_push(stack,data,f) \
    (lbs_list_push_back ((stack), (data), (f)))
#define      lbs_stack_pop(stack)      (lbs_list_pop_back (stack))
#define      lbs_stack_get_len(stack)  (lbs_list_meta_get_len (stack))
#define      lbs_stack_get_data(stack) \
    (lbs_list_get_data (lbs_list_meta_get_last (stack)))

#endif /* LBS_LIST_DISABLE_STACK */


#ifndef LBS_LIST_DISABLE_QUEUE

typedef      LbsListMeta LbsQueue;
#define      lbs_queue_new(f)          (lbs_list_meta_new (f))
#define      lbs_queue_free(queue)     (lbs_list_meta_free (queue))
#define      lbs_queue_enqueue(queue,data,f) \
    (lbs_list_push_back ((queue), (data), (f)))
#define      lbs_queue_dequeue(queue)  (lbs_list_pop_front (queue))
#define      lbs_queue_get_len(queue)  (lbs_list_meta_get_len (queue))
#define      lbs_queue_get_front(queue) \
    (lbs_list_get_data (lbs_list_meta_get_first (queue)))
#define      lbs_queue_get_back(queue) \
    (lbs_list_get_data (lbs_list_meta_get_last (queue)))
#define      lbs_queue_get_data(queue) (lbs_queue_get_front (queue))

#endif /* LBS_LIST_DISABLE_QUEUE */


#endif /* LBS_LIST_H */
