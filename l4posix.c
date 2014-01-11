/* vim: set sw=4 ts=4 sts=4 et: */
#define _POSIX_C_SOURCE 200809L
#define _XOPEN_SOURCE
#include "l4posix.h"

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

char* lbs_posix_strcat (const char* str, ...) {
    va_list ap;
    char* strp;
    char* strnow;
    char* newstr;
    int len = strlen (str);

    va_start (ap, str);
    while ((strp = va_arg (ap, char*)) != NULL) {
        len += strlen (strp);
    }
    va_end (ap);

    newstr = malloc (len + 1);

    va_start (ap, str);
    strnow = stpcpy (newstr, str);
    while ((strp = va_arg (ap, char*)) != NULL) {
        strnow = stpcpy (strnow, strp);
    }
    newstr[len] = '\0';
    va_end (ap);

    return newstr;
}

int lbs_posix_add_fd (int fd, int fdflags) {
    int orgfd = fcntl (fd, F_GETFD);
    if (orgfd < 0) {
        return -1;
    }
    return fcntl (fd, F_SETFD, orgfd | fdflags);
}

int lbs_posix_del_fd (int fd, int fdflags) {
    int orgfd = fcntl (fd, F_GETFD);
    if (orgfd < 0) {
        return -1;
    }
    return fcntl (fd, F_SETFD, orgfd & ~(fdflags));
}

int lbs_posix_add_fl (int fd, int flflags) {
    int orgfl = fcntl (fd, F_GETFL);
    if (orgfl < 0) {
        return -1;
    }
    return fcntl (fd, F_SETFL, orgfl | flflags);
}

int lbs_posix_del_fl (int fd, int flflags) {
    int orgfl = fcntl (fd, F_GETFL);
    if (orgfl < 0) {
        return -1;
    }
    return fcntl (fd, F_SETFL, orgfl & ~(flflags));
}

char* lbs_posix_readlink (const char* filename) {
    struct stat st;
    if (lstat (filename, &st) < 0) {
        return NULL;
    }
    size_t bufsize = st.st_size ? st.st_size : 8192;
    char* buf = malloc (bufsize + 1);
    if (buf == NULL) {
        return NULL;
    }
    ssize_t written = readlink (filename, buf, bufsize);
    if (written < 0) {
        free (buf);
        return NULL;
    }
    buf[written] = '\0';
    return buf;
}

char* lbs_posix_getcwd (void) {
    char *cwd, *result;
    size_t size = pathconf (".", _PC_PATH_MAX);

    size = size > 0 ? size : 256;
    size = size > 8192 ? 8192 : size;
    cwd = malloc (sizeof (char) * size);

    while ((result = getcwd (cwd, size)) == NULL && errno == ERANGE) {
        size *= 2;
        cwd = realloc (cwd, size);
    }

    return cwd;
}

size_t lbs_posix_write_all (int fd, const char* str, size_t size) {
    ssize_t wtn = 0;
    if (size <= 0) {
        size = strlen (str);
    }
    size_t rem = size;
    while (rem > 0) {
        wtn = write (fd, str, rem);
        if (wtn < 0) {
            if (errno != EINTR && errno != EAGAIN) {
                break;
            }
            continue;
        }
        str += wtn;
        rem -= wtn;
    }

    rem = rem > 0 ? rem : 0;
    return size - rem;
}

void lbs_posix_buffer_init (LbsPosixBuffer* buf) {
    buf->buf_start = 0;
    buf->buf_len = 0;
    buf->buf_line = NULL;
    buf->buf_line_len = 0;
    buf->buf_error = false;
    buf->buf_eof = false;
}

void lbs_posix_buffer_clear (LbsPosixBuffer* buf, bool initial) {
    buf->buf_start = 0;
    buf->buf_len = 0;

    if (!initial) {
        free (buf->buf_line);
    }
    buf->buf_line = NULL;
    buf->buf_line_len = 0;
    buf->buf_error = false;
    buf->buf_eof = false;
}

char* lbs_posix_buffer_getline (int fd, LbsPosixBuffer* buf, int delim) {
    if (buf->buf_error || buf->buf_eof) {
        return NULL;
    }

    if (buf->buf_len == 0) {
        int rval = read (fd, buf->buf, LBS_POSIX_BUFFER_SIZE);
        if (rval < 0) {
            if (errno != EAGAIN && errno != EINTR) {
                buf->buf_error = true;
            }
            return NULL;
        }
        if (rval == 0) {
            buf->buf_eof = true;
            return NULL;
        }
        buf->buf_start = 0;
        buf->buf_len = rval;
    }

    int i;
    for (i = 0; i < buf->buf_len; i++) {
        if (buf->buf[buf->buf_start + i] == delim) {
            break;
        }
    }

    int buf_line_start = buf->buf_line_len;
    buf->buf_line_len += i;
    buf->buf_line = realloc (buf->buf_line, buf->buf_line_len + 1);
    memcpy (buf->buf_line + buf_line_start, buf->buf + buf->buf_start, i);
    buf->buf_line[buf->buf_line_len] = '\0';

    /* remove CR if delim is LF and delim is found */
    if (i < buf->buf_len && delim == '\n' && buf->buf_line_len - 1 >= 0 &&
        buf->buf_line[buf->buf_line_len - 1] == '\r') {
        buf->buf_line[buf->buf_line_len - 1] = '\0';
        buf->buf_line_len--;
    }

    int buf_len_saved = buf->buf_len;
    buf->buf_start += i + 1;
    buf->buf_len -= i + 1;
    if (buf->buf_len <= 0) {
        buf->buf_start = 0;
        buf->buf_len = 0;
    }

    if (i < buf_len_saved) {
        /* delim is found */
        char* newstr = buf->buf_line;
        buf->buf_line = NULL;
        buf->buf_line_len = 0;
        memmove (buf->buf, buf->buf + buf->buf_start, buf->buf_len);
        buf->buf_start = 0;
        return newstr;
    }

    return NULL;
}

#define SOCKADDR(x)     ((struct sockaddr*)(x))
#define SOCKADDR_UN(x)  ((struct sockaddr_un*)(x))
#define SOCKADDR_IN(x)  ((struct sockaddr_in*)(x))
#define SOCKADDR_IN6(x) ((struct sockaddr_in6*)(x))

static char* lbs_posix_socket_name (
    int sockfd, int (*getter) (int, struct sockaddr*, socklen_t*)) {
    
    struct sockaddr_storage sock;
    socklen_t socklen = sizeof (sock);

    memset (&sock, 0, socklen);
    if ((*getter)(sockfd, SOCKADDR (&sock), &socklen) < 0) {
        return strdup ("invalid socket");
    }

    int domain = sock.ss_family;
    if (domain == AF_UNIX) {
        return strdup ("local process");
    }

    socklen_t ipstrlen;
    void* ipnet;
    char* ipstr;
    if (domain == AF_INET) {
        ipstrlen = INET_ADDRSTRLEN;
        ipnet = &(SOCKADDR_IN (&sock)->sin_addr);
    } else {
        ipstrlen = INET6_ADDRSTRLEN;
        ipnet = &(SOCKADDR_IN6 (&sock)->sin6_addr);
    }

    ipstr = malloc (ipstrlen);
    if (inet_ntop (domain, ipnet, ipstr, ipstrlen) == NULL) {
        free (ipstr);
        return strdup ("unknown address");
    }

    return ipstr;
}

char* lbs_posix_socket_sockname (int sockfd) {
    return lbs_posix_socket_name (sockfd, getsockname);
}

char* lbs_posix_socket_peername (int sockfd) {
    return lbs_posix_socket_name (sockfd, getpeername);
}

void lbs_posix_exchange_data (int fd[2], LbsPosixProgress prog_cb) {
    int nfds, active;
    fd_set rset, wset;
    fd_set rres, wres;
    LbsPosixBuffer buf[2];

    FD_ZERO (&rset);
    FD_SET (fd[0], &rset);
    FD_SET (fd[1], &rset);
    FD_ZERO (&wset);

    active = 2;
    nfds = (fd[0] > fd[1] ? fd[0] : fd[1]) + 1;
    lbs_posix_buffer_clear (&buf[0], true);
    lbs_posix_buffer_clear (&buf[1], true);

    while (active && (*prog_cb) (fd, buf)) {
        rres = rset;
        wres = wset;
        if (select (nfds, &rres, &wres, NULL, NULL) >= 0) {
            for (int i = 0; i < 2; i++) {
                if (buf[i].buf_len) {
                    /* read buffer full */
                    if (FD_ISSET (fd[!i], &wres)) {
                        int wb = write (fd[!i],
                            buf[i].buf + buf[i].buf_start, buf[i].buf_len);
                        if (wb > 0) {
                            buf[i].buf_start += wb;
                            buf[i].buf_len -= wb;
                            if (!buf[i].buf_len) {
                                FD_CLR (fd[!i], &wset);
                                FD_SET (fd[i], &rset);
                            }
                        } else {
                            if (wb < 0 && (errno == EAGAIN || errno == EINTR)) {
                                continue;
                            } else {
                                FD_CLR (fd[!i], &wset);
                                active = 0;
                            }
                        }
                    }
                } else {
                    /* read buffer empty */
                    if (FD_ISSET (fd[i], &rres)) {
                        int rb = read (fd[i], buf[i].buf, LBS_POSIX_BUFFER_SIZE);
                        if (rb > 0) {
                            buf[i].buf_start = 0;
                            buf[i].buf_len = rb;
                            FD_CLR (fd[i], &rset);
                            FD_SET (fd[!i], &wset);
                        } else {
                            if (rb < 0 && (errno == EAGAIN || errno == EINTR)) {
                                continue;
                            } else{
                                FD_CLR (fd[i], &rset);
                                active = 0;
                            }
                        }
                    }
                }
            }
        }
    }
}
