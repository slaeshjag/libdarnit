#ifndef __BASE64_H__
#define	__BASE64_H__


unsigned int base64calcSizeInBase64(unsigned int size);
unsigned int base64calcSizeInBin(unsigned int size);
char *base64Encode(void *data, unsigned int size);
char *base64Decode(const char *data, unsigned int size, int *outsize);

#endif
