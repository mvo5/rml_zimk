
CFLAGS = -O2 -g -fPIC -Wall -Werror $(shell pkg-config --cflags glib-2.0) -I..
LIBS = -L/usr/lib/freeradius -Wl,-rpath=/usr/lib/freeradius -lpthread -lfreeradius-eap -lfreeradius-radius  ../libmeep/libmeep.a $(shell pkg-config --libs glib-2.0)

all: libmeep test

libmeep:
	make -C ../libmeep

test:
	gcc $(CFLAGS) test_userblacklist.c \
	../zimk_userblacklist.c \
	  -o test_userblacklist $(LIBS)
	gtester test_userblacklist

clean:
	rm -f *.o *.so

.PHONY: clean
