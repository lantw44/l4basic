/* vim: set sw=4 ts=4 sts=4 et: */
#ifndef LBS_ARG_H
#define LBS_ARG_H

#include <l4common.h>
#include <l4array.h>
#include <l4strv.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct LbsArgQuoteStruct {
    char* left;
    char* right;
    bool  super;
} LbsArgQuote;

LbsStrv*       lbs_arg_parse             (const char* str,
                                          const char* delim,
                                          const char* escape,
                                          const LbsArgQuote* quote,
                                          LbsArray**  detail);

typedef struct LbsArgQoptItemStruct {
    char* name;
    char* value;
} LbsArgQoptItem;

typedef struct LbsArgQoptStruct {
    /*< private >*/
    LbsStrv* strv;
    LbsArray* detail;

    /*< public >*/
    size_t len;
    LbsArgQoptItem opts[];
} LbsArgQopt;

LbsArgQopt*    lbs_arg_qopt_new          (const char* str);
void           lbs_arg_qopt_free_generic (void* qopt);
#define lbs_arg_qopt_free(qopt) \
    (lbs_arg_qopt_free_generic (LBS_COMMON_CHECK_TYPE ((qopt), LbsArgQopt*)))

#define lbs_arg_qopt_item_get_name(item) \
    ((LBS_COMMON_CHECK_TYPE (item, LbsArgQoptItem*))->name)
#define lbs_arg_qopt_item_get_value(item) \
    ((LBS_COMMON_CHECK_TYPE (item, LbsArgQoptItem*))->value)
#define lbs_arg_qopt_item_has_name(item) \
    (((LBS_COMMON_CHECK_TYPE (item, LbsArgQoptItem*))->name) != NULL)
#define lbs_arg_qopt_item_has_value(item) \
    (((LBS_COMMON_CHECK_TYPE (item, LbsArgQoptItem*))->value) != NULL)
#define lbs_arg_qopt_item_is_empty(item) \
    ((!lbs_arg_qopt_item_has_name (item)) && \
     (!lbs_arg_qopt_item_has_value (item)))

#define lbs_arg_qopt_get_len(qopt) \
    ((LBS_COMMON_CHECK_TYPE (qopt, LbsArgQopt*))->len)
#define lbs_arg_qopt_get_opt(qopt,index) \
    (&((LBS_COMMON_CHECK_TYPE (qopt, LbsArgQopt*))->opts[index]))
#define lbs_arg_qopt_get_name(qopt,index) \
    (lbs_arg_qopt_item_get_name (lbs_arg_qopt_get_opt (qopt, index)))
#define lbs_arg_qopt_get_value(qopt,index) \
    (lbs_arg_qopt_item_get_value (lbs_arg_qopt_get_opt (qopt, index)))
#define lbs_arg_qopt_has_name(qopt,index) \
    (lbs_arg_qopt_item_has_name (lbs_arg_qopt_get_opt (qopt, index)))
#define lbs_arg_qopt_has_value(qopt,index) \
    (lbs_arg_qopt_item_has_value (lbs_arg_qopt_get_opt (qopt, index)))
#define lbs_arg_qopt_is_empty(qopt,index) \
    (lbs_arg_qopt_item_is_empty (lbs_arg_qopt_get_opt (qopt, index)))

#ifdef __cplusplus
}
#endif

#endif /* LBS_ARG_H */
