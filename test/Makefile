
include ../common.mk

TEST_CFLAGS = $(CFLAGS) -I..
TEST_LIBS = $(LIBS)

test:
	gcc $(TEST_CFLAGS) test_userblacklist.c \
	../zimk_userblacklist.c \
	  -o test_userblacklist $(TEST_LIBS)
	gtester test_userblacklist

clean:
	rm -f *.o *.so

.PHONY: clean
