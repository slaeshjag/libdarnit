#include "darnit.h"


int EXPORT_THIS darnitUtf8CharLengthGet(const char *str) {
	return utf8GetValidatedCharLength(str);
}


unsigned int EXPORT_THIS darnitUtf8UnicodeDecode(const char *str) {
	return utf8GetChar(str);
}


int EXPORT_THIS darnitUtf8UnicodeEncode(unsigned int ch, char *str, int buf_len) {
	return utf8Encode(ch, str, buf_len);
}


int EXPORT_THIS darnitUtf8UnicodeEncodedLength(unsigned int ch) {
	return utf8EncodedLength(ch);
}


int EXPORT_THIS darnitUtf8UnicodeCharactersInString(const char *str) {
	return utf8GetGlyphsInString(str);
}


int EXPORT_THIS darnitUtf8UnicodeCharactersInStringCounted(const char *str, int buf_len) {
	return utf8CountedStringSize(str, buf_len);
}


int EXPORT_THIS darnitUtf8UnicodeCharacterStart(const char *str, unsigned int unicode_pos) {
	return utf8FindCharIndex(str, unicode_pos);
}
