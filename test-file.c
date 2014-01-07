/* vim: set sw=4 ts=4 sts=4 et: */
#undef NDEBUG
#define _POSIX_C_SOURCE 200809L
#include <l4array.h>
#include <l4file.h>

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main () {
    int p[2];
    assert (pipe (p) >= 0);

    pid_t pid = fork ();
    if (pid < 0) {
        return 1;
    } else if (pid > 0){
        FILE* fp = fopen ("test-file.c", "rb");
        assert (fp != NULL);
        FILE* fw = fdopen (p[1], "wb");
        assert (fw != NULL);

        close (p[0]);

        LbsArray* line;
        while ((line = lbs_file_read_file_line (fp)) != NULL && !feof (fp)) {
            fputs (lbs_array_get_data (line), fw);
            putc ('\n', fw);
            lbs_array_unref (line);
        }
        lbs_array_unref (line);

        fclose (fp);
        fclose (fw);
    } else {
        close (p[1]);
        assert (dup2 (p[0], 0) >= 0);
        execlp ("cmp", "cmp", "test-file.c", "-", LBS_COMMON_NULL_PTR);
        assert (0);
    }

    int status;
    wait (&status);
    assert (WIFEXITED (status) && WEXITSTATUS (status) == 0);

    puts ("test_file => PASS!");
    return 0;
}
