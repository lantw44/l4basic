/* vim: set sw=4 ts=4 sts=4 et: */
#ifndef LBS_SYSDEP_H
#define LBS_SYSDEP_H

#include <l4common.h>

char*   lbs_sysdep_get_executable     (void);
char*   lbs_sysdep_get_resource       (const char* filename);

#endif /* LBS_SYSDEP_H */
