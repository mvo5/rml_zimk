
include ../common.mk

TEST_CFLAGS = $(CFLAGS) $(shell pkg-config --cflags glib-2.0) -I..
TEST_LIBS = $(LIBS) ../libmeep/libmeep.a $(shell pkg-config --libs glib-2.0)

all: libmeep test

libmeep:
	make -C ../libmeep

test:
	gcc $(TEST_CFLAGS) test_userblacklist.c \
	../zimk_userblacklist.c \
	  -o test_userblacklist $(TEST_LIBS)
	gtester test_userblacklist

clean:
	rm -f *.o *.so

.PHONY: clean
