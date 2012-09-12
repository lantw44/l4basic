#ifndef L4LIB_ARG_PARSER
#define L4LIB_ARG_PARSER

#ifdef __cplusplus
extern "C" {
#endif

char** l4arg_toargv(const char*, const char*, const char*, const char*);

typedef struct l4lib_qarg {
	char* arg_name;
	char* arg_value;
} L4QARG;

L4QARG* l4qarg_parse(const char*);
void l4qarg_free(L4QARG*);
#define l4qarg_n(qargitem) ((qargitem).arg_name)
#define l4qarg_v(qargitem) ((qargitem).arg_value)

#ifdef __cplusplus
}
#endif

#endif
