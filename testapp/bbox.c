#include <darnit/darnit.h>

#include <stdio.h>
#include <stdlib.h>


int main(int argc, char **argv) {
	DARNIT_BBOX *bbox;
	int ret[5], i;

	darnitInit("BBox test", "bbox-test");
	
	bbox = darnitBBoxNew(5);
	darnitBBoxEntryAdd(bbox, 10, 10, 32, 32);
	darnitBBoxEntryAdd(bbox, 100, 10, 32, 32);
	darnitBBoxEntryAdd(bbox, 400, 10, 32, 32);
	darnitBBoxEntryAdd(bbox, 800, 400, 32, 32);
	darnitBBoxEntryAdd(bbox, 500, 30, 32, 32);

	fprintf(stderr, "Testing entire screen: ");
	for (i = darnitBBoxCollTest(bbox, 0, 0, 800, 480, ret, 5); i > 0; i--)
		fprintf(stderr, "%i ", ret[i-1]);
	fprintf(stderr, "\n");
	fprintf(stderr, "Testing corner %i %i\n", 10, 0);
	for (i = darnitBBoxCollTest(bbox, 10, 10, 100, 100, ret, 5); i > 0; i--)
		fprintf(stderr, "%i ", ret[i-1]);
	fprintf(stderr, "\n");

	return 0;
}
