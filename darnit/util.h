#ifndef __UTIL_H__
#define	__UTIL_H__

#include <time.h>

#ifdef _WIN32
	#include <winsock2.h>
#else
	#include <arpa/inet.h>
#endif


unsigned int utilHtonl(unsigned int val);
unsigned int utilNtohl(unsigned int val);
char *utilPathTranslate(const char *path);
int utilStringToIntArray(const char *str, const char *delimiter, int *dest, int max_tokens);
void utilBlockToHostEndian(unsigned int *block, int elements);
unsigned int utilStringSum(const char *str);


typedef struct {
	int			sine[3600];
} UTIL;

void utilInit();
int utilSine(int angle);
#define utilCos(x) utilSine(900+(x))
#define UTIL_PI 31


#endif
