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


static void add_refs(xmlDocPtr doc, xmlNodePtr cur, int **ref, int *refs) {
	for (cur = cur->xmlChildrenNode; cur; cur = cur->next) {
		if (!strcmp((const char *) cur->name, "property")) {
			(*ref) = realloc((*ref), sizeof(**ref) * (++(*refs)));
			(*ref)[(*refs) - 1] = ref_add((const char *) xmlGetProp(cur, (const xmlChar*) "name"), (const char *) xmlGetProp(cur, (const xmlChar*) "value"));
		}
	}

	return;
}


void *parse_layer_data(xmlDocPtr doc, xmlNodePtr cur) {
	void *data, *out;
	int outsize;

	data = NULL;

	for (cur = cur->xmlChildrenNode; cur; cur = cur->next) {
		if (!strcmp((const char *) cur->name, "data")) {
			data = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			if (!strcmp((const char *) xmlGetProp(cur, (const xmlChar*) "encoding"), "base64"))
				out = base64Decode(data, strlen(data), &outsize);
			else if (!strcmp((const char *) xmlGetProp(cur, (const xmlChar*) "encoding"), "csv")) {
				/* FIXME: Implement csv decode */
			}

			xmlFree(data);
			if (!xmlGetProp(cur, (const xmlChar*) "compression")) {
				/* No compression */
			} else if (!strcmp((const char *) xmlGetProp(cur, (const xmlChar*) "compression"), "zlib"))
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
		if (t < map_info.tileset[i].firstgid)
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
		if (gid != find_tileset(data[i]))
			ret = -1;
		data[i] -= map_info.tileset[gid].firstgid;
	}

	return (ret > -1) ? gid : -1;
}


void parse_layer(xmlDocPtr doc, xmlNodePtr cur) {
	int l, ts, w, h;
	unsigned int *data;

	data = parse_layer_data(doc, cur);
	w = atoi((const char *) xmlGetProp(cur, (const xmlChar*) "width"));
	h = atoi((const char *) xmlGetProp(cur, (const xmlChar*) "height"));
	ts = gidmap(data, w, h);

	if (strcmp((const char *) xmlGetProp(cur, (const xmlChar*) "name"), "collision")) {
		ldmz_main->layers++;
		layer = realloc(layer, sizeof(*layer) * ldmz_main->layers);
		l = ldmz_main->layers - 1;
		layer[l].width = w;
		layer[l].height = h;
		layer[l].tile = data;
		layer[l].ref = NULL;
		layer[l].refs = 0;
		layer[l].ts = ts;
		inject_ref("NAME", (const char *) xmlGetProp(cur, (const xmlChar*) "name"), &layer[l].ref, &layer[l].refs);
		for (cur = cur->xmlChildrenNode; cur; cur = cur->next)
			if (!strcmp((const char *) cur->name, "properties"))
				add_refs(doc, cur, &layer[l].ref, &layer[l].refs);
		inject_ref(NULL, NULL, &layer[l].ref, &layer[l].refs);
	} else {
		l = ldmz_main->layers - 1;
	}

	return;
}


int parse_tileset(xmlDocPtr doc, xmlNodePtr cur) {
	int t, id;
	xmlNodePtr bah;

	t = map_info.tilesets++;
	map_info.tileset = realloc(map_info.tileset, sizeof(*map_info.tileset) * map_info.tilesets);
	map_info.tileset[t].firstgid = atoi((const char *) xmlGetProp(cur, (const xmlChar*) "firstgid"));
	map_info.tileset[t].tile_w = atoi((const char *) xmlGetProp(cur, (const xmlChar*) "tilewidth"));
	map_info.tileset[t].tile_h = atoi((const char *) xmlGetProp(cur, (const xmlChar*) "tileheight"));
	map_info.tileset[t].ref = NULL;
	map_info.tileset[t].refs = 0;

	for (cur = cur->xmlChildrenNode; cur; cur = cur->next) {
		if (!strcmp((const char *) cur->name, "image")) {
			/* TODO: Do something with image */
		} else if (!strcmp((const char *) cur->name, "tile")) {
			id = atoi((const char *) xmlGetProp(cur, (const xmlChar*) "id"));
			fprintf(stderr, "ID: %i\n", id);
			/* TODO: Fix this */
			for (bah = cur->xmlChildrenNode; bah; bah = bah->next)
				if (!strcmp((const char *) bah->name, "properties"))
					add_refs(doc, bah, &map_info.tileset[t].ref, &map_info.tileset[t].refs);
			inject_ref(NULL, NULL, &map_info.tileset[t].ref, &map_info.tileset[t].refs);
		}
	}

	return 1;
}


void parse_object(xmlDocPtr doc, xmlNodePtr cur) {
	int o;

	o = map_info.objects++;
	map_info.object = realloc(map_info.object, sizeof(*map_info.object) * map_info.objects);
	map_info.object[o].x = atoi((const char *) xmlGetProp(cur, (const xmlChar*) "x")) / map_info.tileset[layer[ldmz_main->layers - 1].ts].tile_w;
	map_info.object[o].y = atoi((const char *) xmlGetProp(cur, (const xmlChar*) "y")) / map_info.tileset[layer[ldmz_main->layers - 1].ts].tile_h;
	map_info.object[o].l = ldmz_main->layers - 1;
	map_info.object[o].ref = NULL;
	map_info.object[o].refs = 0;
	
	for (cur = cur->xmlChildrenNode; cur; cur = cur->next)
		if (!strcmp((const char *) cur->name, "properties"))
			add_refs(doc, cur, &map_info.object[o].ref, &map_info.object[o].refs);
	inject_ref("NAME", (const char *) xmlGetProp(cur, (const xmlChar*) "name"), &map_info.object[o].ref, &map_info.object[o].refs);
	if (xmlGetProp(cur, (const xmlChar*) "type"))
		inject_ref("TYPE", (const char *) xmlGetProp(cur, (const xmlChar*) "type"), &map_info.object[o].ref, &map_info.object[o].refs);
	inject_ref(NULL, NULL, &map_info.object[o].ref, &map_info.object[o].refs);

	return;

}


void parse_objectgroup(xmlDocPtr doc, xmlNodePtr cur) {
	for (cur = cur->xmlChildrenNode; cur; cur = cur->next) {
		if (!strcmp((const char *) cur->name, "object"))
			parse_object(doc, cur);
	}

	return;
}


int parse_map(xmlDocPtr doc, xmlNodePtr cur) {
	
	ldmz_main->version = (strcmp((const char *) xmlGetProp(cur, (const xmlChar*) "orientation"), "orthogonal")) ? LDMZ_VERSION_ORTHO : LDMZ_VERSION_ISOM;
	map_info.tile_w = atoi((const char *) xmlGetProp(cur, (const xmlChar*) "tilewidth"));
	map_info.tile_h = atoi((const char *) xmlGetProp(cur, (const xmlChar*) "tileheight"));


	for (cur = cur->xmlChildrenNode; cur; cur = cur->next) {
		if (!strcmp("layer", (const char *) cur->name))
			parse_layer(doc, cur);
		if (!strcmp("tileset", (const char *) cur->name))
			parse_tileset(doc, cur);
		if (!strcmp("objectgroup", (const char *) cur->name))
			parse_objectgroup(doc, cur);
		if (!strcmp("properties", (const char *) cur->name)) {
			add_refs(doc, cur, &map_info.map_ref, &map_info.map_refs);
			inject_ref(NULL, NULL, &map_info.map_ref, &map_info.map_refs);
		}
	}

	return 1;
}


void map_write(const char *fname) {
	FILE *fp;

	if (!(fp = fopen(fname, "wb+"))) {
		fprintf(stderr, "Unable to open %s\n", fname);
		exit(-1);
	}

	
	return;
}


int main(int argc, char **argv) {
	int i;
	xmlDocPtr doc;
	xmlNodePtr cur;

	if (argc <3) {
		fprintf(stderr, "Usage: %s <TMX file> <Output LDMZ>\n", argv[0]);
		return -1;
	}

	if (!(doc = xmlParseFile(argv[1]))) {
		fprintf(stderr, "Unable to parse %s\n", argv[1]);
		return -1;
	}

	if (!(cur = xmlDocGetRootElement(doc))) {
		fprintf(stderr, "TMX %s is invalid (empty)\n", argv[1]);
		return -1;
	}

	if (strcmp((const char *) cur->name, "map")) {
		fprintf(stderr, "%s is not a TMX\n", argv[1]);
		return 0;
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
	parse_map(doc, cur);
	
	fprintf(stderr, "%i refs\n", map_info.refs);
	for (i = 0; i < map_info.refs; i++) {
		if (map_info.ref_d[i].ref[0] < 0) {
			fprintf(stderr, "STOP\n");
			continue;
		}
		fprintf(stderr, "%s: %s\n", map_info.ref + map_info.ref_d[i].ref[0], map_info.ref + map_info.ref_d[i].ref[1]);
	}

	map_write(argv[2]);

	return 0;
}
