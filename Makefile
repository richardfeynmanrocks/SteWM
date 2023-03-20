PREFIX?=/usr/X11R6
CFLAGS?=-Os -pedantic -Wall

all:
	$(CC) $(CFLAGS) -I$(PREFIX)/include stewm.c -L$(PREFIX)/lib -lX11 -o stewm

clean:
	rm -f tinywm
