#include "tmxconv.h"
#include "base64.h"
#include <string.h>
#include <assert.h>


struct ldmz_main *ldmz_main;
struct layer_info *layer;
union {
	int		i;
	char		c[4];
} src_u, dst_u;

struct map_info {
	int		tile_w;
	int		tile_h;

	struct {
			const char *name;
			int firstgid;
			int tile_w;
			int tile_h;
			int *ref;
			int refs;
	} *tileset;
	int		tilesets;

	struct {
			int x;
			int y;
			int l;
			int *ref;
			int refs;
	} *object;
	int 		objects;
	
	char		*ref;
	struct {
			int ref[2];
	} *ref_d;
	int		refs;
	int		ref_size;

	int		*map_ref;
	int		map_refs;
} map_info;


void endian_convert(int *buff, int size) {
	int i, j;

	src_u.i = 1;
	if (!src_u.c[0])
		return;
	for (i = 0; i < size; i++) {
		src_u.i = buff[i];
		for (j = 0; j < 4; j++)
			dst_u.c[j] = src_u.c[3-j];
		buff[i] = dst_u.i;
	}
	
	return;
}


void *inflate_data(void *data_in, int *data_size) {
	int in_size, ret, i, outavail, last;
	z_stream zstr;
	void *outdata;

	in_size = *data_size;
	*data_size = 0;

	zstr.zalloc = Z_NULL;
	zstr.zfree = Z_NULL;
	zstr.opaque = Z_NULL;
	zstr.avail_in = 0;
	zstr.next_in = Z_NULL;
	if ((ret = inflateInit(&zstr)) != Z_OK)
		return NULL;
	
	outdata = malloc(65536);
	outavail = 65536;
	zstr.avail_in = in_size;
	zstr.next_in = data_in;
	zstr.avail_out = outavail;
	zstr.next_out = outdata;
	for (i = last = 0;; ) {
		last = zstr.avail_out;
		assert((ret = inflate(&zstr, Z_NO_FLUSH)) != Z_STREAM_ERROR);
		switch (ret) {
			case Z_NEED_DICT:
			case Z_DATA_ERROR:
			case Z_MEM_ERROR:
				inflateEnd(&zstr);
				free(outdata);
				return NULL;
			default:
				break;
		}

		i += (last - zstr.avail_out);
		if (zstr.avail_out) {
			*data_size = i;
			inflateEnd(&zstr);
			outdata = realloc(outdata, i);
			return outdata;
		}

		if (outavail - i < 65536) {
			outavail += 65536;
			outdata = realloc(outdata, outavail);
		}

		zstr.next_out = outdata + i;
	}

	return 0;
}


void *deflate_data(void *data_in, int data_size, unsigned int *data_out_size) {
	z_stream zstr;
	void *data_out = malloc(data_size + 128);

	zstr.zalloc = Z_NULL;
	zstr.zfree = Z_NULL;
	zstr.opaque = Z_NULL;
	deflateInit(&zstr, 9);
	zstr.avail_in = data_size;
	zstr.next_in = data_in;
	zstr.avail_out = data_size + 128;
	zstr.next_out = data_out;
	deflate(&zstr, Z_FINISH);
	*data_out_size = data_size + 128 - zstr.avail_out;
	deflateEnd(&zstr);

	return data_out;
}


int ref_add(const char *key, const char *val) {
	int r;

	map_info.ref_d = realloc(map_info.ref_d, sizeof(*map_info.ref_d) * (map_info.refs + 1));
	r = map_info.refs++;
	if (key && val) {
		map_info.ref = realloc(map_info.ref, map_info.ref_size + strlen(key) + strlen(val) + 2);
		strcpy(map_info.ref + map_info.ref_size, key);
		map_info.ref_d[r].ref[0] = map_info.ref_size;
		map_info.ref_size += (strlen(key) + 1);
		map_info.ref_d[r].ref[1] = map_info.ref_size;
		strcpy(map_info.ref + map_info.ref_size, val);
		map_info.ref_size += (strlen(val) + 1);
	} else
		map_info.ref_d[r].ref[0] = map_info.ref_d[r].ref[1] = -1;

	return r;
}


static void inject_ref(const char *key, const char *val, int **ref, int *refs) {
	(*ref) = realloc((*ref), sizeof(**ref) * (++(*refs)));
	(*ref)[(*refs) - 1] = ref_add(key, val);

	return;
}


static void add_refs(mxml_node_t *tree, int **ref, int *refs) {
	for (tree = mxmlGetFirstChild(tree); tree; tree = mxmlGetNextSibling(tree)) {
		if (!mxmlGetElement(tree))
			continue;
		if (!strcmp(mxmlGetElement(tree), "property")) {
			(*ref) = realloc((*ref), sizeof(**ref) * (++(*refs)));
			(*ref)[(*refs) - 1] = ref_add(mxmlElementGetAttr(tree, "name"), mxmlElementGetAttr(tree, "value"));
		}
	}

	return;
}


void *parse_layer_data(mxml_node_t *tree) {
	void *out, *data;
	int outsize, nn;

	data = NULL;
	nn = 0;

	for (tree = mxmlGetFirstChild(tree); tree; tree = mxmlGetNextSibling(tree)) {
		if (!mxmlGetElement(tree))
			continue;
		if (!strcmp(mxmlGetElement(tree), "data")) {
			data = (void *) mxmlGetText(tree, &nn);
			if (!strcmp(mxmlElementGetAttr(tree, "encoding"), "base64"))
				out = base64Decode(data, strlen(data), &outsize);
			else if (!strcmp(mxmlElementGetAttr(tree, "encoding"), "csv")) {
				/* FIXME: Implement csv decode */
			}

			if (!mxmlElementGetAttr(tree, "compression")) {
				/* No compression */
			} else if (!strcmp(mxmlElementGetAttr(tree, "compression"), "zlib"))
				data = inflate_data(out, &outsize);
			else {
				/* FIXME: Implement other compression methods */
			}

			free(out);
		}
	}
	

	return data;
}


unsigned int find_tileset(unsigned int t) {
	int i;

	for (i = 0; i < map_info.tilesets; i++) {
		if ((int) t < map_info.tileset[i].firstgid)
			return i - 1;
	}

	return i - 1;
}


int gidmap(unsigned int *data, int w, int h) {
	int i, gid, ret;

	gid = -1;
	ret = 1;

	for (i = 0; i < w * h; i++) {
		if (data[i] == 0)
			continue;
		if (gid < 0)
			gid = find_tileset(data[i]);
		if ((unsigned int) gid != find_tileset(data[i]))
			ret = -1;
		data[i] -= map_info.tileset[gid].firstgid;
	}

	return (ret > -1) ? gid : -1;
}


void parse_layer(mxml_node_t *tree) {
	int l, ts, w, h, i;
	unsigned int *data;

	data = parse_layer_data(tree);
	w = atoi(mxmlElementGetAttr(tree, "width"));
	h = atoi(mxmlElementGetAttr(tree, "height"));
	ts = gidmap(data, w, h);

	if (strcmp(mxmlElementGetAttr(tree, "name"), "collision")) {
		ldmz_main->layers++;
		layer = realloc(layer, sizeof(*layer) * ldmz_main->layers);
		l = ldmz_main->layers - 1;
		layer[l].width = w;
		layer[l].height = h;
		layer[l].tile = data;
		layer[l].ref = NULL;
		layer[l].refs = 0;
		layer[l].ts = ts;
		inject_ref("NAME", mxmlElementGetAttr(tree, "name"), &layer[l].ref, &layer[l].refs);
		for (tree = mxmlGetFirstChild(tree); tree; tree = mxmlGetNextSibling(tree)) {
			if (!mxmlGetElement(tree))
				continue;
			if (!strcmp(mxmlGetElement(tree), "properties"))
				add_refs(tree, &layer[l].ref, &layer[l].refs);
		}
		inject_ref(NULL, NULL, &layer[l].ref, &layer[l].refs);
	} else {
		l = ldmz_main->layers - 1;
		for (i = 0; i < (int) (layer[l].width * layer[l].height); i++)
			layer[l].tile[i] |= (data[i] << 16);
		free(data);
	}

	return;
}


int parse_tileset(mxml_node_t *tree) {
	int t, id;
	mxml_node_t *bah;

	t = map_info.tilesets++;
	map_info.tileset = realloc(map_info.tileset, sizeof(*map_info.tileset) * map_info.tilesets);
	map_info.tileset[t].firstgid = atoi(mxmlElementGetAttr(tree, "firstgid"));
	map_info.tileset[t].tile_w = atoi(mxmlElementGetAttr(tree, "tilewidth"));
	map_info.tileset[t].tile_h = atoi(mxmlElementGetAttr(tree, "tileheight"));
	map_info.tileset[t].ref = NULL;
	map_info.tileset[t].refs = 0;

	for (tree = mxmlGetFirstChild(tree); tree; tree = mxmlGetNextSibling(tree)) {
		if (!mxmlGetElement(tree))
			continue;
		if (!strcmp(mxmlGetElement(tree), "image")) {
			/* TODO: Do something with image */
		} else if (!strcmp(mxmlGetElement(tree), "tile")) {
			id = atoi(mxmlElementGetAttr(tree, "id"));
			/* TODO: Fix this */
			for (bah = mxmlGetFirstChild(tree); bah; bah = mxmlGetNextSibling(bah)) {
				if (!mxmlGetElement(bah))
					continue;
				if (!strcmp(mxmlGetElement(bah), "properties"))
					add_refs(bah, &map_info.tileset[t].ref, &map_info.tileset[t].refs);
			}
			inject_ref(NULL, NULL, &map_info.tileset[t].ref, &map_info.tileset[t].refs);
		}
	}

	return 1;
}


void parse_object(mxml_node_t *tree_n) {
	int o;

	o = map_info.objects++;
	map_info.object = realloc(map_info.object, sizeof(*map_info.object) * map_info.objects);
	map_info.object[o].x = atoi(mxmlElementGetAttr(tree_n, "x")) / map_info.tile_w;
	map_info.object[o].y = atoi(mxmlElementGetAttr(tree_n, "y")) / map_info.tile_h;
	map_info.object[o].l = ldmz_main->layers - 1;
	map_info.object[o].ref = NULL;
	map_info.object[o].refs = 0;
	
	for (tree_n = mxmlGetFirstChild(tree_n); tree_n; tree_n = mxmlGetNextSibling(tree_n)) {
		if (!mxmlGetElement(tree_n))
			continue;
		if (!strcmp(mxmlGetElement(tree_n), "properties"))
			add_refs(tree_n, &map_info.object[o].ref, &map_info.object[o].refs);
	}
	inject_ref("NAME", mxmlElementGetAttr(tree_n, "name"), &map_info.object[o].ref, &map_info.object[o].refs);
	if (mxmlElementGetAttr(tree_n, "type"))
		inject_ref("TYPE", mxmlElementGetAttr(tree_n, "type"), &map_info.object[o].ref, &map_info.object[o].refs);
	inject_ref(NULL, NULL, &map_info.object[o].ref, &map_info.object[o].refs);

	return;

}


void parse_objectgroup(mxml_node_t *tree) {
	for (tree = mxmlGetFirstChild(tree); tree; tree = mxmlGetNextSibling(tree)) {
		if (!mxmlGetElement(tree))
			continue;
		if (!strcmp(mxmlGetElement(tree), "object"))
			parse_object(tree);
	}

	return;
}


int parse_map(mxml_node_t *tree) {
	int nn;
	
	ldmz_main->version = (!strcmp(mxmlElementGetAttr(tree, "orientation"), "orthogonal")) ? LDMZ_VERSION_ORTHO : LDMZ_VERSION_ISOM;
	map_info.tile_w = atoi(mxmlElementGetAttr(tree, "tilewidth"));
	map_info.tile_h = atoi(mxmlElementGetAttr(tree, "tileheight"));

	nn = 0;
	for (tree = mxmlGetFirstChild(tree); tree; tree = mxmlGetNextSibling(tree)) {
		if (!mxmlGetElement(tree))
			continue;
		if (!strcmp("layer", mxmlGetElement(tree)))
			parse_layer(tree);
		if (!strcmp("tileset", mxmlGetElement(tree)))
			parse_tileset(tree);
		if (!strcmp("objectgroup", mxmlGetElement(tree)))
			parse_objectgroup(tree);
		if (!strcmp("properties", mxmlGetElement(tree))) {
			add_refs(tree, &map_info.map_ref, &map_info.map_refs);
			inject_ref("generator", TMXCONV_VERSION, &map_info.map_ref, &map_info.map_refs);
			inject_ref(NULL, NULL, &map_info.map_ref, &map_info.map_refs);
			nn = 1;
		}
	}
	
	if (!nn) {
		inject_ref("ldmz-generator", TMXCONV_VERSION, &map_info.map_ref, &map_info.map_refs);
		inject_ref(NULL, NULL, &map_info.map_ref, &map_info.map_refs);
	}

	return 1;
}


void map_write(const char *fname) {
	FILE *fp;
	void *strtablez, *refz, *layerz, *objectz;
	struct ldmz_object *object;
	struct ldmz_layer *layer_n;
	int i;

	if (!(fp = fopen(fname, "wb+"))) {
		fprintf(stderr, "Unable to open %s\n", fname);
		exit(-1);
	}

	ldmz_main->stringtable_size = map_info.ref_size;
	ldmz_main->stringtable_ref_size = map_info.refs * sizeof(*map_info.ref_d);
	ldmz_main->stringtable_ref_count = map_info.refs;
	ldmz_main->map_ref_index = map_info.map_ref[0];
	ldmz_main->objects = map_info.objects;

	object = malloc(sizeof(*object) * map_info.objects);
	for (i = 0; i < map_info.objects; i++) {
		object[i].x = map_info.object[i].x;
		object[i].y = map_info.object[i].y;
		object[i].l = map_info.object[i].l;
		object[i].ref = map_info.object[i].ref[0];
	}

	layer_n = malloc(sizeof(*layer_n) * ldmz_main->layers);
	for (i = 0; i < (int) ldmz_main->layers; i++) {
		layer_n[i].tile_w = map_info.tile_w;
		layer_n[i].tile_h = map_info.tile_h;
		if (ldmz_main->version == LDMZ_VERSION_ORTHO)
			layer_n[i].layer_off_x = layer_n[i].layer_off_y = 0;
		else {
			layer_n[i].layer_off_x = map_info.tileset[layer[i].ts].tile_h;
			layer_n[i].layer_off_y = 0;
		}
		layer_n[i].ref = layer[i].ref[0];
		layer_n[i].w = layer[i].width;
		layer_n[i].h = layer[i].height;

		endian_convert((int *) layer[i].tile, layer[i].width * layer[i].height);
		layer[i].tile_z = deflate_data(layer[i].tile, layer[i].width * layer[i].height * sizeof(unsigned int), &layer_n[i].layer_size_z);
	}

	endian_convert((int *) map_info.ref_d, ldmz_main->stringtable_ref_count * 2);
	endian_convert((int *) object, ldmz_main->objects * 4);
	endian_convert((int *) layer_n, ldmz_main->layers * 8);

	strtablez = deflate_data(map_info.ref, map_info.ref_size, &ldmz_main->stringtable_compr);
	refz = deflate_data(map_info.ref_d, map_info.refs * sizeof(*map_info.ref_d), &ldmz_main->stringtable_ref_compr);
	objectz = deflate_data(object, map_info.objects * sizeof(*object), &ldmz_main->objects_compr);
	layerz = deflate_data(layer_n, ldmz_main->layers * sizeof(*layer_n), &ldmz_main->layer_header_compr);

	free(map_info.ref);
	free(map_info.ref_d);
	endian_convert((int *) ldmz_main, 12);

	fwrite(ldmz_main, 12, 4, fp);
	endian_convert((int *) ldmz_main, 12);
	endian_convert((int *) layer_n, ldmz_main->layers * 8);
	fwrite(strtablez, ldmz_main->stringtable_compr, 1, fp);
	fwrite(refz, ldmz_main->stringtable_ref_compr, 1, fp);
	fwrite(objectz, ldmz_main->objects_compr, 1, fp);
	fwrite(layerz, ldmz_main->layer_header_compr, 1, fp);

	for (i = 0; i < (int) ldmz_main->layers; i++)
		fwrite(layer[i].tile_z, layer_n[i].layer_size_z, 1, fp);

	
	return;
}


int main(int argc, char **argv) {
	int i;
	FILE *fp;
	mxml_node_t *tree;

	if (argc <3) {
		fprintf(stderr, "Usage: %s <TMX file> <Output LDMZ>\n", argv[0]);
		return -1;
	}

	if (!(fp = fopen(argv[1], "r"))) {
		fprintf(stderr, "Unable to open %s\n", argv[1]);
		return -1;
	}

	if (!(tree = mxmlLoadFile(NULL, fp, MXML_NO_CALLBACK))) {
		fprintf(stderr, "TMX %s is invalid (empty)\n", argv[1]);
		return -1;
	}

//	fclose(fp);


	tree = mxmlGetNextSibling(mxmlGetFirstChild(tree));
	if (!tree) {
		fprintf(stderr, "%s is not a TMX\n", argv[1]);
		return -1;
	}

	ldmz_main = calloc(1, sizeof(*ldmz_main));
	ldmz_main->magic = LDMZ_MAGIC;
	map_info.tileset = NULL;
	map_info.tilesets = 0;
	map_info.ref = NULL;
	map_info.ref_d = NULL;
	map_info.refs = 0;
	map_info.ref_size = 0;
	map_info.map_ref = NULL;
	map_info.map_refs = 0;
	parse_map(tree);
	
	fprintf(stderr, "%i refs\n", map_info.refs);
	for (i = 0; i < map_info.refs; i++)
		if (map_info.ref_d[i].ref[0] < 0)
			continue;

	map_write(argv[2]);

	return 0;
}
