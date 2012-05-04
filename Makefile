CFLAGS += -Wall -fPIC -Wl,-soname,libdarnit.so -shared -I./include -O0 -g -std=c99 -fvisibility=hidden
SOURCE = src/render.c src/darnit.c src/input.c src/text.c src/menutk.c src/audio.c src/imgload.c src/sprite.c src/bbox.c src/render_tilemap.c src/tilemap.c src/dynlib.c src/socket.c
LDFLAGS += -lSDL -lmodplay -lvorbisfile -lpng -L./bin -ldl

default:
	mkdir -p bin
	cd deps/modplay && make
	$(CC) $(CFLAGS) $(SOURCE) -o bin/libdarnit.so $(LDFLAGS) -lGL

pandora:
	mkdir -p bin
	cd deps/modplay && make
	$(CC) $(CFLAGS) $(SOURCE) -o bin/libdarnit.so $(LDFLAGS) -lGLES_CM -lEGL -lX11

clean:
	rm -Rf bin
	rm -f testapp/testapp

install:
	cp bin/*.so /usr/local/lib/
	cp include/darnit.h /usr/local/include
