#include "darnit.h"

unsigned int bitwiseRoundUpToPow2(unsigned int val) {
	unsigned int i;

	if (val > 0x80000000)
		return 0;
	for (i = 1; i < val; i<<=1);

	return i;
}


unsigned int bitwiseGetPower(unsigned int val) {
	unsigned int i, pow;

	pow = bitwiseRoundUpToPow2(val);
	for (i = 0; (1 << i) < pow; i++);

	return i;
}
