#include "tun.h"
#include "../config/config.h"
#include <string.h>             // memset memcpy
#include <fcntl.h>              // open
#include <linux/if.h>           // ifreq
#include <stdbool.h>            // true
#include <stdio.h>              // snprintf
#include <sys/ioctl.h>          // ioctl
#include <linux/if_tun.h>       // TUNSETIFF
#include <unistd.h>             // close
#include <stddef.h>             // NULL

const char *tun_setup(struct config *config)
{
    memset(config, 0, sizeof(struct config));
    if ((config->tun = open("/dev/net/tun", O_RDWR)) < 0) {
        return "Unable to open dev.\n";
    }
    struct ifreq ifr = {
        .ifr_flags = IFF_TUN,
        .ifr_name = "tun"
    };
    int id = 0;
    while (true) {
        int size = snprintf(ifr.ifr_name, IFNAMSIZ, "tun%d", id);
        if (ioctl(config->tun, TUNSETIFF, (void *)&ifr) < 0) {
            id++;
            if (id > 9) {
                close(config->tun);
                return "Unable to allocate tun.\n";
            }
        } else {
            memcpy(config->tun_name, ifr.ifr_name, size);
            break;
        }
    }
    return NULL;
}
