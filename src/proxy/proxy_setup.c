#include "proxy.h"
#include "../config/config.h"
#include <stddef.h>             // NULL
#define _GNU_SOURCE             // memrchr
#include <string.h>             // strlen memcpy
#undef _GNU_SOURCE
#include <netdb.h>              // addrinfo getaddrinfo
#include <sys/socket.h>         // AF_INET AF_INET6
#include <netinet/in.h>         // sockaddr_in sockaddr_in6
#include <stdlib.h>             // malloc free

static void *proxy_close_arg;
static void proxy_close();

const char *proxy_setup(struct config *config, char *proxy)
{
    char *port = NULL;
    size_t len = strlen(proxy);
    char *pos = memrchr(proxy, ':', len);
    if (pos) {
        *pos = 0;
        port = pos + 1;
        len = pos - proxy;
    }
    if (proxy[0] == '[' && proxy[len - 1] == ']') {
        proxy[len - 1] = 0;
        proxy = proxy + 1;
    }
    struct addrinfo *res;
    if (getaddrinfo(proxy, port, NULL, &res)) {
        return "Could not resolve proxy address.\n";
    }
    if (!res || (res->ai_family != AF_INET && res->ai_family != AF_INET6)) {
        freeaddrinfo(res);
        return "Unknown proxy family\n.";
    }
    size_t size;
    if (res->ai_family == AF_INET6) {
        size = sizeof(struct sockaddr_in6);
    } else {
        size = sizeof(struct sockaddr_in);
    }
    config->proxy_addr = malloc(size);
    if (!config->proxy_addr) {
        freeaddrinfo(res);
        return "Unable to allocate memory.\n";
    }
    memcpy(config->proxy_addr, res->ai_addr, size);
    freeaddrinfo(res);
    proxy_close_arg = config->proxy_addr;
    atexit(proxy_close);
    return NULL;
}

static void proxy_close()
{
    free(proxy_close_arg);
}
