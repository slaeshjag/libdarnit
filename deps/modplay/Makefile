LDFLAGS += -Wl,-soname,libmodplay.so -shared -fPIC
LIBS=

default:
	$(CC) *.c -o ../../bin/libmodplay.so $(CFLAGS) $(LDFLAGS)
	strip ../../bin/libmodplay.so
