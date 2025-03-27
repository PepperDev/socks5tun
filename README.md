# SOCKS5 tun

Create a tun network interface forwarding all traffic through SOCKS5 proxy.

## Download

Download the releases packages for your archictecture, for now it's only for Linux, all binaries are statically built, not requring to match the libc version.

## Build

```
./build
```

## Usage

Standalone usage:
```
bin/socks5tun socks5-hostname-or-ip:1080
```
*Note:* all traffic but the proxy ip itself will go through the tunnel, thus the proxy must be reliable.

## Usage with TOR

Since TOR forward a local connection through the TOR network, it would loopback not working as expected, thus you need to isolate both, we suggest using docker, podman, containerd or any other container isonation mechanism.

We provide a script to do it with docker, if you don't use docker you can read it and adapt for your preffered container provider.

```
./tor
```

## To Do list

* Timeout UDP when there is no traffic for more than five minutes
* Use DNS over TLS (DoT), port 853, to enhance privacy
* User TURN over TLS port 5349 instead of 3478
* Use multi thread and queue, maching the host cores count
* Allow custom CIDR, DNS, and TURN
* Detect IPv6 support throught SOCKS5 automatically
* Detect UDP throught SOCKS5 before using TURN (e.g.: accessing DNS or STUN)
