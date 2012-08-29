#include <stdlib.h>
#include <l4darr.h>

L4DA2* l4da2_create_setmax(int itemsize, L4XY len, L4XY maxlen){
	if(len.x < 0 || len.y < 0 || maxlen.x < 0 || maxlen.y < 0)
		return NULL;
	}
	L4DA2* arr = (L4DA2*)malloc();
}
