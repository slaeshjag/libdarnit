#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	FILE *in, *out;
	unsigned int t, r;

	if (argc <3) {
		fprintf(stderr, "Usage: %s <raw texture> <mapfile>\n");
		return -1;
	}


	if ((in = fopen(argv[1], "r")) == NULL) {
		fprintf(stderr, "unable to open %s\n", argv[1]);
		return -1;
	}


	if ((out = fopen(argv[2], "w")) == NULL) {
		fprintf(stderr, "unable to open %s\n", argv[2]);
		return -1;
	}

	fread(&t, 4, 1, in);
	fwrite(&t, 4, 1, out);
	fread(&t, 4, 1, in);
	fwrite(&t, 4, 1, out);

	while (!feof(in)) {
		fread(&t, 4, 1, in);
		r = (t & 0xF0) >> 4;
		r |= (t & 0xF000) >> 12;
		r |= (t & 0xF00000) >> 20;
		fwrite(&r, 4, 1, out);
	}

	
	return 0;
}
