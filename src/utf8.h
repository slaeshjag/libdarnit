#ifndef __UTF8_H__
#define	__UTF8_H__

#define			UTF8_REPLACEMENT_CHAR		0xFFFD
#define			UTF8_CHAR_LIMIT			0x10FFFF


int utf8GetCharLength(const unsigned char *str);
int utf8Validate(const unsigned char *str);
unsigned int utf8GetChar(const char *str_s);
int utf8GetValidatedCharLength(const char *str_s);


#endif
