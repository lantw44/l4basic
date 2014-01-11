/* vim: set sw=4 ts=4 sts=4 et: */

#include "l4sysdep.h"
#include "l4posix.h"
#include "l4str.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#ifdef __FreeBSD__
# include <sys/sysctl.h>
#endif

char* lbs_sysdep_get_executable (void) {
	char* myexec;

#ifdef __FreeBSD__
	int fb_mib[4] = { CTL_KERN, KERN_PROC, KERN_PROC_PATHNAME, -1 };
	int fb_rval;
	size_t fb_size = 256;
	myexec = malloc (fb_size);
	while ((fb_rval = sysctl (fb_mib, 4, myexec, &fb_size, NULL, 0)) < 0 &&
		 errno == ENOMEM) {

		fb_size *= 2;
		myexec = realloc (myexec, fb_size);
	}
	if (fb_rval >= 0) {
		return myexec;
	} else {
		free (myexec);
	}
#endif

	if ((myexec = lbs_posix_readlink ("/proc/self/exe")) != NULL) {
		return myexec;
	}
	if ((myexec = lbs_posix_readlink ("/proc/curproc/exe")) != NULL) {
		return myexec;
	}
	if ((myexec = lbs_posix_readlink ("/proc/curproc/file")) != NULL) {
		return myexec;
	}

	return NULL;
}

char* lbs_sysdep_get_resource (const char* filename) {
	char *myexec, *myres;
	bool myexec_static = false;

	myexec = lbs_sysdep_get_executable ();
	if (myexec == NULL) {
		myexec = lbs_posix_getcwd ();
		if (myexec == NULL) {
			myexec = "./";
			myexec_static = true;
		}
	} else {
		char* dirsep = strrchr (myexec, '/');
		if (dirsep != NULL && dirsep != myexec) {
			*dirsep = '\0';
		}
	}

	if (lbs_str_has_suffix (myexec, "/")) {
		myres = lbs_posix_strcat (myexec, filename, (char*)NULL);
	} else {
		myres = lbs_posix_strcat (myexec, "/", filename, (char*)NULL);
	}
	if (!myexec_static) {
		free (myexec);
	}

	return myres;
}
