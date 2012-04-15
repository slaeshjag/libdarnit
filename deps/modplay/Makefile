LDFLAGS += -Wl,-soname,libmodplay.so -shared -fPIC
LIBS=

default:
	$(CC) *.c -o ../../bin/libmodplay.so $(CFLAGS)
	strip ../../bin/libmodplay.so
