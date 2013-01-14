#ifndef __DARNIT_UTIL_H__
#define	__DARNIT_UTIL_H__


unsigned int d_util_htonl(unsigned int arg);
unsigned int d_util_htohl(unsigned int arg);
char *d_util_path_translate(const char *path);
int d_util_string_to_int_array(const char *str, const char *delimiter, int *dest, int max_tokens);
void d_util_endian_convert(unsigned int *block, int elements);


#endif
