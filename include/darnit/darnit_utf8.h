#ifndef __DARNIT_UTF8_H__
#define	__DARNIT_UTF8_H__

int darnitUtf8CharLenghtGet(const char *str);
unsigned int darnitUtf8UnicodeDecode(const char *str);
int darnitUtf8UnicodeEncode(unsigned int ch, char *str, int buf_len);
int darnitUtf8UnicodeEncodedLength(unsigned int ch);
int darnitUtf8UnicodeCharactersInString(const char *str);
int darnitUtf8UnicodeCharactersInStringCounted(const char *str, int buf_len);
int darnitUtf8UnicodeCharacterStart(const char *str, unsigned int unicode_pos);


#endif
