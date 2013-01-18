#include "darnit.h"


int EXPORT_THIS d_utf8_valid(const char *str) {
	return utf8Validate(str);
}


int EXPORT_THIS d_utf8_char_length(const char *str) {
	return utf8GetValidatedCharLength(str);
}


unsigned int EXPORT_THIS d_utf8_decode(const char *str) {
	return utf8GetChar(str);
}


int EXPORT_THIS d_utf8_encode(unsigned int ch, char *str, int buf_len) {
	return utf8Encode(ch, str, buf_len);
}


int EXPORT_THIS d_utf8_encoded_length(unsigned int ch) {
	return utf8EncodedLength(ch);
}


int EXPORT_THIS d_utf8_chars_in_string(const char *str) {
	return utf8GetGlyphsInString(str);
}


int EXPORT_THIS d_utf8_chars_in_string_limited(const char *str, int buf_len) {
	return utf8CountedStringSize(str, buf_len);
}


int EXPORT_THIS d_utf8_char_pos(const char *str, unsigned int unicode_pos) {
	return utf8FindCharIndex(str, unicode_pos);
}
