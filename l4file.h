/* vim: set sw=4 ts=4 sts=4 et: */
#ifndef LBS_FILE_H
#define LBS_FILE_H

#include <l4array.h>
#include <stdio.h>

#define   lbs_file_read_stdin_line() \
    (lbs_file_read_file_line_delim (stdin, '\n'))
#define   lbs_file_read_stdin_line_delim(delim) \
    (lbs_file_read_file_line_delim (stdin, (delim)))
#define   lbs_file_read_file_line(infile) \
    (lbs_file_read_file_line_delim ((infile), '\n'))
LbsArray* lbs_file_read_file_line_delim (FILE* infile, int delim);

#endif /* LBS_FILE_H */
