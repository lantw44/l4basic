#ifndef LBS_COMMON_H
#define LBS_COMMON_H

#include <stddef.h>

#ifdef __STDC_VERSION__
# include <stdbool.h>
# if __STDC_VERSION__ >= 201112L
#  define LBS_COMMON_ISO_C11
#  define LBS_COMMON_CHECK_TYPE(x,type) (_Generic ((x), type: (x)))
# else
#  define LBS_COMMON_ISO_C99
#  define LBS_COMMON_CHECK_TYPE(x,type) (x)
# endif
#else
# define bool  char
# define true  1
# define false 0
# define inline
# define LBS_COMMON_ISO_C89
# define LBS_COMMON_CHECK_TYPE(x,type) (x)
#endif /* __STDC_VERSION__ */

#define LBS_COMMON_NULL_PTR ((void*)NULL)
#define LBS_COMMON_DEFINE_GETTER(ns,xt,m,mt) \
    static inline mt ns ## _get_ ## m (xt x) { \
        return x->m; \
    }
#define LBS_COMMON_DEFINE_SETTER(ns,xt,m,mt) \
    static inline void ns ## _set_ ## m (xt x, mt v) { \
        x->m = v; \
    }

#endif /* LBS_COMMON_H */
