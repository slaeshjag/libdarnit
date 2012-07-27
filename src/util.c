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
