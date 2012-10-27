#include "darnit.h"


unsigned int EXPORT_THIS darnitUtilHtonl(unsigned int arg) {
	return utilHtonl(arg);
}


unsigned int EXPORT_THIS darnitUtilNtohl(unsigned int arg) {
	return utilNtohl(arg);
}


char EXPORT_THIS *darnitUtilPathTranslate(const char *path) {
	return utilPathTranslate(path);
}


int EXPORT_THIS darnitUtilStringToIntArray(const char *str, const char *delimiter, int *dest, int max_tokens) {
	return utilStringToIntArray(str, delimiter, dest, max_tokens);
}
