

CFLAGS = -O2 -g -fPIC
LIBS = -L/usr/lib/freeradius -Wl,-rpath=/usr/lib/freeradius -lpthread -lfreeradius-eap -lfreeradius-radius 


rlm_zimk.so: rlm_zimk.c
	gcc $(CFLAGS) -c rlm_zimk.c 
	gcc -shared -export-dynamic -o rlm_zimk.so rlm_zimk.o $(LIBS)

clean:
	rm -f *.o *.so

.PHONY: clean
