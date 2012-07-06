CFLAGS = -c -Wall -g -fPIC
DEBUG = -g -O0
RELEASE = -O3

defualt:
	$(CC) $(CFLAGS) truetype.c
	$(CC) $(CFLAGS) stb_image.c
	$(CC) $(CFLAGS) stb_vorbis.c

clean:
	rm -f *.o
