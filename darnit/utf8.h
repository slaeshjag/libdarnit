#ifndef __UTF8_H__
#define	__UTF8_H__

#define			UTF8_REPLACEMENT_CHAR		0xFFFD
#define			UTF8_CHAR_LIMIT			0x10FFFF


int utf8GetCharLength(const unsigned char *str);
int utf8Validate(const unsigned char *str);
unsigned int utf8GetChar(const char *str_s);
int utf8GetValidatedCharLength(const char *str_s);
int utf8FindCharIndex(const char *str_s, unsigned int pos);
const char *utf8FindStartByCharacterPos(const char *str_s, unsigned int pos);
int utf8CountedStringSize(const char *str_s, unsigned int chars);
int utf8GetGlyphsInString(const char *str_s);
int utf8EncodedLength(unsigned int ch);
int utf8Encode(unsigned int ch, char *str_s, int buf_len);


#endif
