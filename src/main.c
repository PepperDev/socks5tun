#include "config/config.h"
#include "proxy/proxy.h"
#include "tun/tun.h"
#include "route/route.h"
#include "dns/dns.h"
#include "error/error.h"

int main(int argc, char *argv[])
{
    char *proxy;
    if (argc <= 1) {
        return error_log("Proxy address required.\n");
    }
    proxy = argv[1];
    struct config config = (struct config) { 0 };
    const char *error;
    if ((error = proxy_setup(&config, proxy))) {
        return error_log(error);
    }
    if ((error = tun_setup(&config))) {
        return error_log(error);
    }
    if ((error = route_setup(&config))) {
        return error_log(error);
    }
    if ((error = dns_setup(&config))) {
        return error_log(error);
    }
    if ((error = proxy_loop(&config))) {
        return error_log(error);
    }
    // list and save current route, both IPv4 and IPv6, to be restored when the program exit
    // delete all routes and add the default route to the tun gateway with metric 0, a mask 24 route for the local private network created by the tun, and a /32 route for the gateway used by the proxy
    // monitor route changes and restore, can happen in case a removable network interface device is hot plugged
    // connect to socks when receive TCP traffic
    // connect to turn over socks when receive UDP traffic
    return 0;
}
