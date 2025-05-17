#ifndef CONFIG_H
#define CONFIG_H

struct config {
    void *proxy_addr;
    char tun_name[5];
    int tun;
} __attribute__((packed));

#endif
