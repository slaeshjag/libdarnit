#include "darnit.h"


int EXPORT_THIS utf8CharLenghtGet(const char *str) {
	return utf8GetValidatedCharLength(str);
}


unsigned int EXPORT_THIS utf8UnicodeDecode(const char *str) {
	return utf8GetChar(str);
}


int EXPORT_THIS utf8UnicodeEncode(unsigned int ch, char *str, int buf_len) {
	return utf8Encode(ch, str, buf_len);
}


int EXPORT_THIS utf8UnicodeEncodedLength(unsigned int ch) {
	return utf8EncodedLength(ch);
}


int EXPORT_THIS utf8UnicodeCharactersInString(const char *str) {
	return utf8GetGlyphsInString(str);
}


int EXPORT_THIS utf8UnicodeCharactersInStringCounted(const char *str, int buf_len) {
	return utf8CountedStringSize(str, buf_len);
}


int EXPORT_THIS utf8UnicodeCharacterStart(const char *str, unsigned int unicode_pos) {
	return utf8FindCharIndex(str, unicode_pos);
}
