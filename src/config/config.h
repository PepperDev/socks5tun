#ifndef CONFIG_H
#define CONFIG_H

struct config {
    char tun_name[5];
    int tun;
    int udp;
} __attribute__((packed));

#endif
