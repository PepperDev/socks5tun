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
#include <linux/ipv6.h>         // in6_ifreq

static int tun_close_arg;

static const char *tun_allocate(struct config *, struct ifreq *);
static const char *tun_network(struct config *, int *);
static void tun_close();
static const char *tun_mtu(int, struct ifreq *);
static const char *tun_addr(int, struct ifreq *);
static const char *tun_up(int, struct ifreq *);

const char *tun_setup(struct config *config)
{
    if ((config->tun = open("/dev/net/tun", O_RDWR)) < 0) {
        return "Unable to open dev.\n";
    }
    tun_close_arg = config->tun;
    atexit(tun_close);
    struct ifreq ifr = {
        .ifr_flags = IFF_TUN,
        .ifr_name = {0}
    };
    const char *error;
    if ((error = tun_allocate(config, &ifr))) {
        return error;
    }
    int udp;
    if ((error = tun_network(config, &udp))) {
        return error;
    }
    if ((error = tun_mtu(udp, &ifr))) {
        close(udp);
        return error;
    }
    if ((error = tun_addr(udp, &ifr))) {
        close(udp);
        return error;
    }
    // fe80::/64 ipv6 is automatically assigned
    if ((error = tun_up(udp, &ifr))) {
        close(udp);
        return error;
    }
    close(udp);
    return NULL;
}

static const char *tun_allocate(struct config *config, struct ifreq *ifr)
{
    int id = 0;
    for (;;) {
        int size = snprintf(ifr->ifr_name, IFNAMSIZ, "tun%d", id);
        if (ioctl(config->tun, TUNSETIFF, (void *)ifr)) {
            id++;
            if (id > 9) {
                return "Unable to allocate tun.\n";
            }
        } else {
            memcpy(config->tun_name, ifr->ifr_name, size);
            break;
        }
    }
    return NULL;
}

static const char *tun_network(struct config *config, int *udp)
{
    if ((*udp = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        return "Unable to create control UDP socket for IPv4.\n";
    }
    return NULL;
}

static void tun_close()
{
    close(tun_close_arg);
}

static const char *tun_mtu(int udp, struct ifreq *ifr)
{
    ifr->ifr_mtu = 8500;        // default 1500
    if (ioctl(udp, SIOCSIFMTU, (void *)ifr)) {
        return "Unable to set tunnel MTU.\n";
    }
    return NULL;
}

static const char *tun_addr(int udp, struct ifreq *ifr)
{
    ((struct sockaddr_in *)&ifr->ifr_addr)->sin_family = AF_INET;
    ((struct sockaddr_in *)&ifr->ifr_addr)->sin_addr = (struct in_addr) { 0x2100000a };
    if (ioctl(udp, SIOCSIFADDR, (void *)ifr)) {
        return "Unable to allocate tunnel IPv4.\n";
    }
    ((struct sockaddr_in *)&ifr->ifr_netmask)->sin_family = AF_INET;
    ((struct sockaddr_in *)&ifr->ifr_netmask)->sin_addr = (struct in_addr) { 0x00ffffff };
    if (ioctl(udp, SIOCSIFNETMASK, (void *)ifr)) {
        return "Unable to set tunnel IPv4 mask.\n";
    }
    return NULL;
}

static const char *tun_up(int udp, struct ifreq *ifr)
{
    if (ioctl(udp, SIOCGIFFLAGS, (void *)ifr)) {
        return "Unable to fetch tunnel state.\n";
    }
    ifr->ifr_flags |= IFF_UP | IFF_RUNNING;
    if (ioctl(udp, SIOCSIFFLAGS, (void *)ifr)) {
        return "Unable to change tunnel state.\n";
    }
    return NULL;
}
