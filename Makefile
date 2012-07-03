CFLAGS += -Wall -I./src -fPIC -Wl,-soname,libdarnit.so -shared -I./include  -I./deps -O0 -g -fvisibility=hidden
SOURCE = src/api/*.c src/render.c src/input.c src/text.c src/menutk.c src/audio.c src/imgload.c src/sprite.c src/bbox.c src/render_tilemap.c src/tilemap.c src/dynlib.c src/socket.c src/mt_sprite.c src/audio_load.c src/utf8.c src/bitwise.c deps/*.o
LDFLAGS += -lSDL -lvorbisfile -lmodplug -L./bin -ldl

default:
	mkdir -p bin
	cd deps && make
	$(CC) $(CFLAGS) $(SOURCE) -o bin/libdarnit.so $(LDFLAGS) -lGL

pandora:
	mkdir -p bin
	cd deps && make
	$(CC) $(CFLAGS) $(SOURCE) -o bin/libdarnit.so $(LDFLAGS) -lGLES_CM -lEGL -lX11

clean:
	rm -Rf bin
	rm -f testapp/testapp
	cd deps && make clean

install:
	cp bin/*.so /usr/local/lib/
	cp -Rv include/darnit /usr/local/include
