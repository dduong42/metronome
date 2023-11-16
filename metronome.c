#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>

static void
usage(void)
{
    fprintf(stderr, "Usage: metronome <tempo>\n");
    exit(EXIT_FAILURE);
}

static long
my_strtol(char const s[static 1], int base)
{
    char *endptr = 0;

    long n = strtol(s, &endptr, base);

    if (errno) {
        perror("strtol");
        exit(EXIT_FAILURE);
    }

    if (endptr == s) usage();
    return n;
}

static void
my_nanosleep(struct timespec const req[static 1])
{
    struct timespec sleep_time = *req;
    struct timespec rem = { 0 };

    while (nanosleep(&sleep_time, &rem) == -1) {
        if (errno != EINTR) {
            perror("nanosleep");
            exit(EXIT_FAILURE);
        }
        sleep_time = rem;
        rem.tv_sec = 0;
        rem.tv_nsec = 0;
    }
}

int
main(int argc, char *argv[argc+1])
{
    if (argc != 2) usage();

    long tempo = my_strtol(argv[1], 10);
    if (tempo <= 0) usage();

    long long dodo = 60000000000 / tempo;

    struct timespec const sleep_time = {
        .tv_sec = dodo / 1000000000,
        .tv_nsec = dodo % 1000000000
    };

    for (;;) {
        if (putchar('\a') == EOF) return EXIT_FAILURE;
        if (fflush(stdout) == EOF) {
            perror("fflush");
            return EXIT_FAILURE;
        }
        my_nanosleep(&sleep_time);
    }
    return EXIT_SUCCESS;
}
