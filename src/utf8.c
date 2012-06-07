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
	
	for (i = 1; i < j; j++)
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

	for (i = 1; i < len; i++)
		chr += ((unsigned int) str[i] & 0x3F) << ((i*6) - (7 - len));
	
	if (chr > UTF8_CHAR_LIMIT)
		return UTF8_REPLACEMENT_CHAR;
	return chr;
}
