#ifndef PROXY_H
#define PROXY_H

struct config;

const char *proxy_setup(struct config *, char *);
const char *proxy_loop(struct config *);

#endif
