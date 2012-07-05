#ifndef __DARNIT_UTF8_H__
#define	__DARNIT_UTF8_H__

int utf8CharLenghtGet(const char *str);
unsigned int utf8UnicodeDecode(const char *str);
int utf8UnicodeEncode(unsigned int ch, char *str, int buf_len);
int utf8UnicodeEncodedLength(unsigned int ch);
int utf8UnicodeCharactersInString(const char *str);
int utf8UnicodeCharactersInStringCounted(const char *str, int buf_len);
int utf8UnicodeCharacterStart(const char *str, unsigned int unicode_pos);


#endif
