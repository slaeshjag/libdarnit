#ifndef __UTIL_H__
#define	__UTIL_H__

#ifdef _WIN32
	#include <winsock2.h>
#else
	#include <arpa/inet.h>
#endif


unsigned int utilHtonl(unsigned int val);
unsigned int utilNtohl(unsigned int val);


#endif
