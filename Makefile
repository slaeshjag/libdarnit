CFLAGS += -Wall -fPIC -Wl,-soname,libdarner.so -shared -I./include -O0 -g
SOURCE = src/render.c src/darner.c src/input.c src/text.c src/menutk.c src/audio.c src/imgload.c src/sprite.c src/bbox.c src/render_tilemap.c src/tilemap.c
LDFLAGS += -lSDL -lmodplay -lvorbisfile -lpng -L./bin

default:
	$(CC) $(CFLAGS) $(SOURCE) -o bin/libdarner.so $(LDFLAGS) -lGL

pandora:
	$(CC) $(CFLAGS) $(SOURCE) -o bin/libdarner.so $(LDFLAGS) -lGLES_CM -lEGL -lX11

clean:
	rm -f bin/libdarner.so
	rm -f testapp/testapp
