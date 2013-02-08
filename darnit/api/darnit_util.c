#include "darnit.h"


unsigned int EXPORT_THIS d_util_htonl(unsigned int arg) {
	return utilHtonl(arg);
}


unsigned int EXPORT_THIS d_util_ntohl(unsigned int arg) {
	return utilNtohl(arg);
}


char EXPORT_THIS *d_util_path_translate(const char *path) {
	return utilPathTranslate(path);
}


int EXPORT_THIS d_util_string_to_int_array(const char *str, const char *delimiter, int *dest, int max_tokens) {
	return utilStringToIntArray(str, delimiter, dest, max_tokens);
}


void EXPORT_THIS d_util_endian_convert(unsigned int *block, int elements) {
	utilBlockToHostEndian(block, elements);

	return;
}


int EXPORT_THIS d_util_sin(int angle) {
	return utilSine(angle);
}


const char EXPORT_THIS *d_str_null(const char *str) {
	if (!str)
		return NULL;
	if (!strstr("NULL", str))
		return NULL;
	return str;
}
