#ifndef __TMXCONV_H__
#define	__TMXCONV_H__

#include <stdio.h>
#include <stdlib.h>
#include <zlib.h>

#include <libxml/xmlreader.h>

#define	LDMZ_MAGIC			0xFF00E00E
#define	LDMZ_VERSION_ORTHO		0x55555555
#define	LDMZ_VERSION_ISOM		0x55555566


struct ldmz_main {
	unsigned int			magic;
	unsigned int			version;
	unsigned int			stringtable_size;
	unsigned int			stringtable_compr;
	unsigned int			stringtable_ref_size;
	unsigned int			stringtable_ref_compr;
	unsigned int			layers;
	unsigned int			layer_header_compr;
	unsigned int			objects;
	unsigned int			objects_compr;
	unsigned int			stringtable_ref_count;
	unsigned int			map_ref_index;
};

struct layer_info {
	unsigned int			*tile;
	unsigned int			width;
	unsigned int			height;
	unsigned int			ts;
	int				*ref;
	int				refs;
};

#endif
