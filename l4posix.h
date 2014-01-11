/* vim: set sw=4 ts=4 sts=4 et: */
#ifndef LBS_POSIX_H
#define LBS_POSIX_H

#include <l4common.h>
#include <sys/types.h>

char*    lbs_posix_strcat           (const char* str, ...);
int      lbs_posix_add_fd           (int fd, int fdflags);
int      lbs_posix_del_fd           (int fd, int fdflags);
int      lbs_posix_add_fl           (int fd, int flflags);
int      lbs_posix_del_fl           (int fd, int flflags);
char*    lbs_posix_readlink         (const char* filename);
char*    lbs_posix_getcwd           (void);
size_t   lbs_posix_write_all        (int fd, const char* str, size_t size);
char*    lbs_posix_socket_sockname  (int sockfd);
char*    lbs_posix_socket_peername  (int sockfd);


#define LBS_POSIX_BUFFER_SIZE 4096

typedef struct {
    char    buf[LBS_POSIX_BUFFER_SIZE];
    off_t   buf_start;
    off_t   buf_len;
    char*   buf_line;
    ssize_t buf_line_len;
    int     buf_error : 1;
    int     buf_eof   : 1;
} LbsPosixBuffer;

void     lbs_posix_buffer_init      (LbsPosixBuffer* buf);
void     lbs_posix_buffer_clear     (LbsPosixBuffer* buf, bool initial);
char*    lbs_posix_buffer_getline   (int fd, LbsPosixBuffer* buf, int delim);

typedef  int (*LbsPosixProgress)    (int fd[2], LbsPosixBuffer buf[2]);
void     lbs_posix_exchange_data    (int fd[2], LbsPosixProgress prog_cb);

#endif /* LBS_POSIX_H */
