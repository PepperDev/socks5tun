#include "route.h"
#include "../config/config.h"
#include <stddef.h>             // NULL

const char *route_setup(struct config *config)
{
    // use netlink instead of /proc/net/route | /proc/net/ipv6_route
    // list and save route to restore on exit
    // compute proxy gateway
    // replace the entire route table - default 10.0.0.1/32 metric 0; 10.0.0.0/24 10.0.0.33; proxy-ip/32 proxy-gateway; default dev tun0; fe80::/64 dev tun0 
    // monitor route changes and recover from it
    return NULL;
}
