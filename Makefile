CFLAGS += -Wall -I./src -fPIC -Wl,-soname,libdarnit.so -shared -I./include -O0 -g -fvisibility=hidden -std=c89 
SOURCE = src/api/*.c src/render.c src/input.c src/text.c src/menutk.c src/audio.c src/imgload.c src/sprite.c src/bbox.c src/render_tilemap.c src/tilemap.c src/dynlib.c src/socket.c src/mt_sprite.c src/audio_load.c
LDFLAGS += -lSDL -lvorbisfile -lmodplug -lpng -L./bin -ldl

default:
	mkdir -p bin
	$(CC) $(CFLAGS) $(SOURCE) -o bin/libdarnit.so $(LDFLAGS) -lGL

pandora:
	mkdir -p bin
	$(CC) $(CFLAGS) $(SOURCE) -o bin/libdarnit.so $(LDFLAGS) -lGLES_CM -lEGL -lX11

clean:
	rm -Rf bin
	rm -f testapp/testapp

install:
	cp bin/*.so /usr/local/lib/
	cp -Rv include/darnit /usr/local/include
