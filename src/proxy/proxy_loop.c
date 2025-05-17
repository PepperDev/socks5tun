#include "proxy.h"
#include "../config/config.h"
#define _POSIX_C_SOURCE         // sigaction
#include <signal.h>             // sigemptyset
#undef _POSIX_C_SOURCE
#include <unistd.h>             // pause
#include <stddef.h>             // NULL
#include <stdlib.h>             // exit EXIT_SUCCESS

static void proxy_signal(int);

const char *proxy_loop(struct config *config)
{
    struct sigaction sa = {
        .sa_handler = proxy_signal,
        .sa_flags = 0
    };
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGINT, &sa, NULL)) {
        return "Unable to hook INT signal action.\n";
    }
    if (sigaction(SIGTERM, &sa, NULL)) {
        return "Unable to hook INT signal action.\n";
    }
    for (;;) {
        pause();
    }
    return NULL;
}

static void proxy_signal(int signal)
{
    exit(EXIT_SUCCESS);
}
