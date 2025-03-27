#include "config/config.h"
#include "tun/tun.h"
#include "error/error.h"

#define DEFAULT "127.0.0.1:9050"

int main(int argc, char *argv[])
{
    const char *proxy;
    if (argc > 1) {
        proxy = argv[1];
    } else {
        proxy = DEFAULT;
    }
    struct config config;
    const char *error = tun_setup(&config);
    if (error) {
        return error_log(error);
    }
    // connect to socks
    // connect to turn
    return 0;
}
