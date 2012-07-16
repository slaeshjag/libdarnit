CFLAGS += -Wall -I./src -fPIC -Wl,-soname,libdarnit.so -shared -I./include  -I./deps -O0 -g -fvisibility=hidden
SOURCE = src/api/*.c src/render.c src/input.c src/text.c src/menutk.c src/audio.c src/imgload.c src/sprite.c src/bbox.c src/render_tilemap.c src/tilemap.c src/dynlib.c src/socket.c src/mt_sprite.c src/audio_load.c src/utf8.c src/bitwise.c src/util.c src/stringtable.c deps/*.o
LDFLAGS += -lSDL -lmodplug -L./bin -ldl

default:
	mkdir -p bin
	make base
	$(CC) $(CFLAGS) $(SOURCE) -o bin/libdarnit.so $(LDFLAGS) -lGL

pandora:
	mkdir -p bin
	make base
	$(CC) $(CFLAGS) $(SOURCE) -o bin/libdarnit.so $(LDFLAGS) -lGLES_CM -lEGL -lX11

raspberrypi:
	mkdir -p bin
	make base
	$(CC) $(CFLAGS) $(SOURCE) -o bin/libdarnit.so $(LDFLAGS) -lGLES_CM -lEGL -lX11 -DHAVE_GLES -DRASPBERRYPI -I/opt/vc/include -L/opt/vc/lib

base:
	cd deps && make
	cd tools && make

clean:
	rm -Rf bin
	rm -f testapp/testapp
	cd deps && make clean
	cd tools && make clean

install:
	cp bin/*.so /usr/local/lib/
	cp bin/darnit-stringtable /usr/local/bin
	cp bin/darnit-mksprite /usr/local/bin
	cp -Rv include/darnit /usr/local/include
	ldconfig

install-yactfeau:
	cp bin/*.so /usr/local/angstrom/arm/arm-angstrom-linux-gnueabi/usr/lib
	cp -Rv include/darnit /usr/local/angstrom/arm/arm-angstrom-linux-gnueabi/usr/include
