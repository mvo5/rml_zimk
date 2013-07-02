
include common.mk

MODULE_CFLAGS = $(CFLAGS)
MODULE_LIBS = $(LIBS) 

rlm_zimk.so: rlm_zimk.c zimk_userblacklist.c
	gcc $(MODULE_CFLAGS) -c rlm_zimk.c 
	gcc -shared -export-dynamic -o rlm_zimk.so rlm_zimk.o $(MODULE_LIBS)

clean:
	rm -f *.o *.so

test:
	make -C test

install: rlm_zimk.so
	install -m 644 -u root -g root rlm_zimk.so /usr/lib/freeradius/

uninstall:
	rm -f /usr/lib/freeradius/rlm_zimk.so

.PHONY: clean test
