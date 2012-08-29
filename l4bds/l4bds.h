#ifndef L4LIB_BASIC_DATA_STRUCTURE
#define L4LIB_BASIC_DATA_STRUCTURE

/*********** list ***********/

struct l4lib_list_node{ /* list 中每一個項目用的資料結構，會有很多個 */
	struct l4lib_list_node* node_prev;
	struct l4lib_list_node* node_next;
	void* node_data;
};

typedef struct l4lib_list{ /* 管理 list 用的，每個 list 只有一個 */
	struct l4lib_list_node* list_first;
	struct l4lib_list_node* list_last;
	struct l4lib_list_node* list_current;
	int list_prevcount;
	int list_nextcount;
	int list_len;
} L4LL;

/* 這兩個常數僅供 l4ll_remove 使用 */
#define L4LL_PREV 1 /* 刪除後移至上一項 */
#define L4LL_NEXT 2 /* 刪除後移至下一項 */

/* 這兩個常數僅供 l4ll_goto 使用 */
#define L4LL_FRONT 3
#define L4LL_BACK 4

L4LL* l4ll_create(void);
void l4ll_free(L4LL*);
void* l4ll_getcur(L4LL*);
void* l4ll_getback(L4LL*);
void* l4ll_getfront(L4LL*);
int l4ll_pushback(L4LL*, void*);
int l4ll_pushfront(L4LL*, void*);
int l4ll_popback(L4LL*);
int l4ll_popfront(L4LL*);
int l4ll_remove(L4LL*, int);
int l4ll_insprev(L4LL*);
int l4ll_insnext(L4LL*);
int l4ll_goto(L4LL*, int, int);
#define l4ll_getlen(list)
#define l4ll_getc(list)
#define l4ll_getpc(list)
#define l4ll_getnc(list)

/*********** stack ***********/

#define l4stack_create()
#define l4stack_push(list, value)
#define l4stack_pop(list)
#define l4stack_getlen(list)
#define l4stack_value(list)
#define l4stack_free(list)

/*********** queue ***********/

#define l4queue_create()
#define l4queue_push(list, value)
#define l4queue_pop(list)
#define l4queue_getlen(list)
#define l4queue_value(list)
#define l4queue_free(list)

#endif
