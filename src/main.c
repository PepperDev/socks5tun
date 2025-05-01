#include "config/config.h"
#include "proxy/proxy.h"
#include "route/route.h"
#include "tun/tun.h"
#include "error/error.h"

#define DEFAULT "127.0.0.1:9050"

int main(int argc, char *argv[])
{
    char *proxy;
    if (argc > 1) {
        proxy = argv[1];
    } else {
        proxy = DEFAULT;
    }
    struct config config = (struct config) { 0 };
    const char *error = proxy_setup(&config, proxy);
    if (error) {
        return error_log(error);
    }
    error = tun_setup(&config);
    if (error) {
        return error_log(error);
    }
    error = route_setup(&config);
    if (error) {
        return error_log(error);
    }
    error = proxy_loop(&config);
    if (error) {
        return error_log(error);
    }
    // get gateway to proxy IP, may be different than default route, need to be done after the DNS resolution, atention to IPv4 and IPv6 related routes must match the resolved IP
    // list and save current route, both IPv4 and IPv6, to be restored when the program exit
    // delete all routes and add the default route to the tun gateway with metric 0, a mask 24 route for the local private network created by the tun, and a /32 route for the gateway used by the proxy
    // monitor route changes and restore, can happen in case a removable network interface device is hot plugged
    // connect to socks when receive TCP traffic
    // connect to turn over socks when receive UDP traffic
    return 0;
}
