/*
Copyright (c) 2011-2013 Steven Arnow
'darnit_utf8.c' - This file is part of libdarnit

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

	1. The origin of this software must not be misrepresented; you must not
	claim that you wrote the original software. If you use this software
	in a product, an acknowledgment in the product documentation would be
	appreciated but is not required.

	2. Altered source versions must be plainly marked as such, and must not be
	misrepresented as being the original software.

	3. This notice may not be removed or altered from any source
	distribution.
*/


#include "darnit.h"


int EXPORT_THIS d_utf8_start_char(unsigned char c) {
	if (!(c & 0x80) || (c & 0xC0) == 0xC0)
		return 0;
	return -1;
}


int EXPORT_THIS d_utf8_valid(const unsigned char *str) {
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
