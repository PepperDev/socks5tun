TARGET  ?= bin/socks5tun
DESTDIR ?= /usr/local

SOURCES = $(patsubst %,src/%.c, \
		error/error_log \
		proxy/proxy_setup \
		tun/tun_setup \
		route/route_setup \
		dns/dns_setup \
		proxy/proxy_loop \
		main \
	)

OBJS    = $(SOURCES:src/%.c=.objs/%.o)
DIRS    = $(patsubst %/,%,$(sort $(dir $(TARGET) $(OBJS))))

CFLAGS  = -std=c99 -pedantic -O3 -Wall
LDFLAGS = -static
LIBS    =

all: $(TARGET)
.PHONY: all clear install

$(TARGET): $(OBJS) | bin
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)
	strip $@

.objs/%.o: src/%.c | .objs $(DIRS)
	$(CC) $(CFLAGS) -c $< -o $@

$(DIRS):
	-mkdir -p $@

clean:
	-rm -rf $(TARGET) .objs

install:
	install -o 0 -g 0 -m 4755 $(TARGET) $(DESTDIR)/bin/
