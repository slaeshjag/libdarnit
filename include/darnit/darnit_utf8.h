#ifndef __DARNIT_UTF8_H__
#define	__DARNIT_UTF8_H__

int d_utf8_char_length(const char *str);
unsigned int d_utf8_decode(const char *str);
int d_utf8_encode(unsigned int ch, char *str, int buf_len);
int d_utf8_encoded_length(unsigned int ch);
int d_utf8_chars_in_string(const char *str);
int d_utf8_chars_in_string_limited(const char *str, int buf_len);
int d_utf8_char_pos(const char *str, unsigned int unicode_pos);


#endif
