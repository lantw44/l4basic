#ifndef L4LIB_DYNAMIC_ARRAY_D2
#define L4LIB_DYNAMIC_ARRAY_D2

/*********** 二維陣列 (其實是用一維陣列來模擬，功能有限) ***********/

typedef struct l4lib_dyn_2darr{
	int arr_itemsize;		/* 每個項目的大小 */
	int arr_lenx;	 	 	/* 陣列 x 方向長度 */
	int arr_leny;  			/* 陣列 y 方向長度 */
	void* arr_data; 	   	/* 資料區 */
} L4DA2 ;

L4DA2* l4da2_create(int, int, int);
void l4da2_free(L4DA2*);
#define l4da2_getlenx(arr) ((arr)->arr_lenx)
#define l4da2_getleny(arr) ((arr)->arr_leny)
#define l4da2_itemsize(arr) ((arr)->arr_itemsize)
#define l4da2_data(arr) ((arr)->arr_data)
#define l4da2_v(arr, type, numx, numy) \
	(*(((type*)((arr)->arr_data))+((numx)*(l4da2_getleny(arr)))+(numy)))
#define l4da2_vp(arr, numx, numy) \
	((void*)(((char*)((arr)->arr_data))+ \
	((arr)->arr_itemsize)*((numx)*(l4da2_getleny(arr))+(numy))))

#endif
