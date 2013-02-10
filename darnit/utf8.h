/*
Copyright (c) 2011-2013 Steven Arnow
'utf8.h' - This file is part of libdarnit

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
