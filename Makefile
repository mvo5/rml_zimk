
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

.PHONY: clean test
