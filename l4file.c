#include "l4file.h"

#include <stdio.h>

LbsArray* lbs_file_read_file_line_delim (FILE* infile, int delim) {
	LbsArray* str = lbs_array_new (sizeof (char));
	if (str == NULL){
		return NULL;
	}

	int c;
	char towrite;
	while ((c = getc (infile)) != delim && !feof (infile)) {
		towrite = c;
		if (lbs_array_append_var (str, towrite) < 0){
			lbs_array_unref (str);
			return NULL;
		}
	}
	towrite = '\0';
	if (lbs_array_append_var (str, towrite) < 0) {
		lbs_array_unref (str);
		return NULL;
	}
	return str;
}
