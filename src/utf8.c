#include "darnit.h"


int utf8GetCharLength(const unsigned char *str) {
	if (!((*str & 0xC0) ^ 0x80))
		return 0;
	if (!(*str & 0x80))
		return 1;
	if (!((*str & 0xE0) ^ 0xC0))
		return 2;
	if (!((*str & 0xF0) ^ 0xE0))
		return 3;
	if (!((*str & 0xF8) ^ 0xF0))
		return 4;
	
	return 0;
}


int utf8Validate(const unsigned char *str) {
	int i, j;

	j = utf8GetCharLength(str);
	
	for (i = 1; i < j; i++)
		if (((str[i] & 0xC0) ^ 0x80))
			return -1;
	if (str[0] == 0xC0 || str[i] == 0xC1)
		return -1;
	if (str[0] >= 0xF5)
		return -1;
	
	return 0;
}


unsigned int utf8GetChar(const char *str_s) {
	const unsigned char *str = (const unsigned char *) str_s;
	int i;
	unsigned int chr, len, shift;

	if ((len = utf8GetCharLength(str)) == 0)
		return UTF8_REPLACEMENT_CHAR;
	if (!utf8Validate(str) == -1)
		return UTF8_REPLACEMENT_CHAR;

	shift = 1;
	if (len > 1) shift += len;
		chr = (*str & (0xFF >> shift));
	chr <<= (len-1) * 6;

	for (i = 1; i < len; i++) 
		chr += ((unsigned int) str[i] & 0x3F) << (len - i - 1) * 6;

	return (chr > UTF8_CHAR_LIMIT) ? UTF8_REPLACEMENT_CHAR : chr;
}


int utf8GetValidatedCharLength(const char *str_s) {
	int len;

	if (*str_s == 0)
		return 0;
	
	len = utf8GetCharLength((const unsigned char *) str_s);
	if (utf8Validate((const unsigned char *) str_s) == -1)
		return 1;
	
	return (len > 0) ? len : 1;
}


int utf8FindCharIndex(const char *str_s, unsigned int pos) {
	int i, j;

	for (i = j = 0; str_s[i] != 0 && j != pos; j++)
		i += utf8GetValidatedCharLength(&str_s[i]);
	return (j != pos) ? -1 : i;
}


const char *utf8FindStartByCharacterPos(const char *str_s, unsigned int pos) {
	int i;

	i = utf8FindCharIndex(str_s, pos);
	return (i != -1) ? &str_s[i] : NULL;
}


int utf8CountedStringSize(const char *str_s, unsigned int chars) {
	int i, j;

	for (i = j = 0; i < chars; i++)
		j += utf8GetValidatedCharLength(&str_s[j]);
	return j;
}


int utf8GetGlyphsInString(const char *str_s) {
	int i, j;

	for (i = j = 0; str_s[i] != 0; j++)
		i += utf8GetValidatedCharLength(&str_s[i]);
	return j;
}


int utf8EncodedLength(unsigned int ch) {
	if (ch < 0x80)
		return 1;
	if (ch < 0x800)
		return 2;
	if (ch < 0x10000)
		return 3;
	return 4;
}


int utf8Encode(unsigned int ch, char *str_s, int buf_len) {
	int i, j;
	unsigned char *str = (unsigned char *) str_s;

	if ((i = j = utf8EncodedLength(ch)) == 1) {
		*str = ch;
		return 1;
	}

	if (buf_len < i)
		return 0;

	for (; j > 1; j--)
		str[j-1] = 0x80 | (0x3F & (ch >> ((i - j) * 6)));

	*str = (~0) << (7 - i);
	*str |= (ch >> (i * 6 - 6));

	return i;
}

