
CFLAGS = -O2 -g -fPIC -Wall -Werror $(shell pkg-config --cflags glib-2.0)
LIBS = -L/usr/lib/freeradius -Wl,-rpath=/usr/lib/freeradius -lpthread -lfreeradius-eap -lfreeradius-radius $(shell pkg-config --libs glib-2.0)
