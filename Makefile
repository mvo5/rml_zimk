
include common.mk

MODULE_CFLAGS = $(CFLAGS)
MODULE_LIBS = $(LIBS) 

rlm_zimk.so: rlm_zimk.o zimk_userblacklist.o
	gcc -shared -export-dynamic -o rlm_zimk.so zimk_userblacklist.o rlm_zimk.o $(MODULE_LIBS)

rlm_zimk.o: rlm_zimk.c
	gcc $(MODULE_CFLAGS) -c rlm_zimk.c

zimk_userblacklist.o: zimk_userblacklist.c
	gcc $(MODULE_CFLAGS) -c zimk_userblacklist.c




clean:
	rm -f *.o *.so

test:
	make -C test

install: rlm_zimk.so
	install -m 644 -o root -g root rlm_zimk.so /usr/lib/freeradius/

uninstall:
	rm -f /usr/lib/freeradius/rlm_zimk.so

.PHONY: clean test install uninstall
