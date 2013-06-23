
CFLAGS = -O2 -g -fPIC -Wall -Werror
LIBS = -L/usr/lib/freeradius -Wl,-rpath=/usr/lib/freeradius -lpthread -lfreeradius-eap -lfreeradius-radius 
