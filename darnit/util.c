#include "darnit.h"


unsigned int utilHtonl(unsigned int val) {
	return htonl(val);
}


unsigned int utilNtohl(unsigned int val) {
	return ntohl(val);
}


char *utilPathTranslate(const char *path) {
	char *new_path;
	#ifdef _WIN32
		char *blol;
	#endif

	if ((new_path = malloc(strlen(path) + 1)) == NULL)
		return NULL;
	strcpy(new_path, path);

	#ifdef _WIN32
		blol = new_path;
		while (1) {
			blol = strstr(blol, "/");
			if (blol != NULL) {
				*blol = '\\';
				blol++;
			} else
				break;
		}
	#endif

	return new_path;
}


int utilStringToIntArray(const char *str, const char *delimiter, int *dest, int max_tokens) {
	int token;

	for (token = 0; token < max_tokens; token++) {
		sscanf(str, "%i", &dest[token]);
		str = strstr(str, delimiter);
		if (str == NULL)
			break;
		str += strlen(delimiter);
	}

	return token;
}


void utilBlockToHostEndian(unsigned int *block, int elements) {
	DARNIT_ENDIAN_CONVERT ec;
	int i;

	if (d->platform.platform & DARNIT_PLATFORM_BIGENDIAN)
		return;
	
	/* OPTIMIZE */
	for (i = 0; i < elements; i++) {
		ec.c[0] = ((unsigned int) block[i] >> 24);
		ec.c[1] = ((unsigned int) block[i] >> 16) & 0xFF;
		ec.c[2] = ((unsigned int) block[i] >> 8) & 0xFF;
		ec.c[3] = block[i] & 0xFF;
		block[i] = ec.i;
	}

	return;
}


unsigned int utilStringSum(const char *str) {
	const unsigned char *str_u = (const unsigned char *) str;
	int i;
	unsigned int sum = 0;

	for (i = 0; str_u[i]; i++)
		sum += str_u[i];
	return sum;
}


void utilInit() {
	int i;

	srandom(time(NULL));

	for (i = 0; i < 3600; i++)
		d->util.sine[i] = sinf(M_PI / 1800 * i) * 65536;
	return;
}


int utilSine(int angle) {
	angle = (angle < 0) ? (3600 + (angle % 3600)) : angle % 3600;

	return d->util.sine[angle];
}
