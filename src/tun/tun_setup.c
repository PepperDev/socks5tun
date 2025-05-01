#include "tun.h"
#include "../config/config.h"
#include <string.h>             // memcpy
#include <fcntl.h>              // open
#include <linux/if.h>           // ifreq
#include <stdio.h>              // snprintf
#include <sys/ioctl.h>          // ioctl
#include <linux/if_tun.h>       // TUNSETIFF
#include <unistd.h>             // close
#include <stddef.h>             // NULL
#include <sys/socket.h>         // socket
#include <netinet/in.h>         // IPPROTO_UDP
#include <stdlib.h>             // atexit

static struct {
    int tun;
    int udp;
} tun_close_arg;
static void tun_close();

const char *tun_setup(struct config *config)
{
    if ((config->tun = open("/dev/net/tun", O_RDWR)) < 0) {
        return "Unable to open dev.\n";
    }
    struct ifreq ifr = {
        .ifr_flags = IFF_TUN,
        .ifr_name = "tun"
    };
    int id = 0;
    for (;;) {
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
    config->udp = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (config->udp == -1) {
        close(config->tun);
        return "Unable to create UDP socket.\n";
    }
    // TODO: setup nic and ipv4 and ipv6
    tun_close_arg.tun = config->tun;
    tun_close_arg.udp = config->udp;
    atexit(tun_close);
    return NULL;
}

static void tun_close()
{
    close(tun_close_arg.udp);
    close(tun_close_arg.tun);
}
