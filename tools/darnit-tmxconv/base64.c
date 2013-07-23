#include <stdlib.h>
#include <stdio.h>
#include <string.h>


static const char convert[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

unsigned int base64calcSizeInBase64(unsigned int size) {
	if (size % 3)
		size += (3 - size % 3);
	size += size/3;

	return size;
}

unsigned int base64calcSizeInBin(unsigned int size) {
	size >>= 2;
	return size*3;
}

char *base64Encode(void *data, unsigned int size) {
	unsigned int i, out, sizein;
	unsigned char *data_out, *data_in, buf[4];

	sizein = size;
	size = base64calcSizeInBase64(size);
	data_in = data;
	if (!(data_out = malloc(sizeof(size+1)))) {
		fprintf(stderr, "base64Encode(): Error: Unable to malloc(%u)\n", size);
		return NULL;
	}
	
	for (i = out = 0; i < sizein; i += 3, out += 4) {
		buf[0] = (data_in[i] & 0xFC) >> 2;
		buf[1] = (data_in[i] & 0x3) << 4;

		if (i+1 < sizein) {
			buf[1] |= (data_in[i+1] & 0xF0) >> 4;
			buf[2] = (data_in[i+1] & 0xF) << 2;
		} else
			buf[2] = 64;
		 
		if (i+2 < sizein) {
			buf[2] |= (data_in[i+2] & 0xC0) >> 6;
			buf[3] = (data_in[i+2] & 0x3F);
		} else
			buf[3] = 64;
		
		data_out[out] = convert[buf[0]];
		data_out[out+1] = convert[buf[1]];
		data_out[out+2] = convert[buf[2]];
		data_out[out+3] = convert[buf[3]];
	}
	
	data_out[size] = 0;

	return (char *) data_out;
}


static unsigned char base64atob(unsigned char a) {
	if (a >= 'A' && a <= 'Z')
		return a - 'A';
	if (a >= 'a' && a <= 'z')
		return a - 'a' + 26; 	// There's 26 letters in the änglisch alphabeta
	if (a >= '0' && a <= '9')
		return (a ^= 0x30) + 52;
	if (a == '+')
		return 62;
	if (a == '/')
		return 63;
	if (a == '=')
		return 0;
	
	fprintf(stderr, "Warning: Bad character '%c' detected, output will be messed up...\n", a);
	return 0xFF;
}


static int validate_data_in(const unsigned char *data) {
	int i;

	for (i = 0; i < 4; i++) {
		if ((data[i] >= 'A' && data[i] <= 'Z') || (data[i] >= 'a' && data[i] <= 'z') || (data[i] >= '0' && data[i] <= '9'))
			continue;
		if (data[i] == '+' || data[i] == '/' || data[i] == '=')
			continue;
		return 0;
	}

	return 1;
}


char *base64Decode(const char *data, unsigned int size, int *outsize) {
	unsigned int sizein;
	int i, out;
	unsigned char *data_out, *data_in;
	
	sizein = size;
	size = base64calcSizeInBin(size);
	data_in = (unsigned char *) data;
	*outsize = 0;

	if ((data_out = calloc(size, 1)) == NULL) {
		fprintf(stderr, "base64Decode(): Unable to malloc(%u)\n", size);
		return NULL;
	}

	for (i = out = 0; i < sizein; i += 4, out += 3) {
		if (!validate_data_in(&data_in[i])) {
			i -= 3;
			out -= 3;
			continue;
		}

		data_out[out] = base64atob(data_in[i]) << 2;
		data_out[out] |= (base64atob(data_in[i+1]) & 0x30) >> 4;
		data_out[out+1] = (base64atob(data_in[i+1]) & 0xF) << 4;
		data_out[out+1] |= (base64atob(data_in[i+2]) & 0x3C) >> 2;
		data_out[out+2] = (base64atob(data_in[i+2]) & 0x3) << 6;
		data_out[out+2] |= (base64atob(data_in[i+3]));
	}

	*outsize = out;
	return (char *) data_out;
}

