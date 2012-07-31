#ifndef __DARNIT_UTIL_H__
#define	__DARNIT_UTIL_H__


unsigned int darnitUtilHtonl(unsigned int arg);
unsigned int darnitUtilNtohl(unsigned int arg);
char *darnitUtilPathTranslate(const char *path);
int darnitUtilStringToIntArray(const char *str, const char *delimiter, int *dest, int max_tokens);

#endif
