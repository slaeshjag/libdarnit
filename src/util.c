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


void utilBlockToHostEndian(void *handle, unsigned int *block, int elements) {
	DARNIT *m = handle;
	DARNIT_ENDIAN_CONVERT ec;
	int i;

	if (m->platform.platform & DARNIT_PLATFORM_BIGENDIAN)
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
