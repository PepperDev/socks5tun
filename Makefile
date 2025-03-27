TARGET  ?= bin/socks5tun
DESTDIR ?= /usr/local

SOURCES = $(patsubst %,src/%.c, \
		error/error_log \
		tun/tun_setup \
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
