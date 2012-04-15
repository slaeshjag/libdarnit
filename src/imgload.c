#include "darnit.h"


int imgloadCheckFormat(const char *fname) {
	FILE *fp;
	char buff[8];
	int i;

	if ((fp = fopen(fname, "r")) == NULL)
		return IMG_IS_BAD;
	for (i = 0; i < 8; i++)
		buff[i] = 0;
	fread(buff, 8, 1, fp);
	fclose(fp);

	return (png_check_sig((unsigned char *) buff, 8) != 0) ? IMG_IS_PNG : IMG_IS_RAW;
}


IMGLOAD_DATA imgloadBAD(const char *fname) {
	IMGLOAD_DATA data;

	data.w = data.h = 0;
	data.img_data = NULL;

	fprintf(stderr, "Unable to load image %s\n", fname);

	return data;
}


IMGLOAD_DATA imgloadRAW(const char *fname) {
	IMGLOAD_DATA data;
	FILE *fp;


	fp = fopen(fname, "r");
	fread(&data, sizeof(unsigned int), 2, fp);

	if ((data.img_data = malloc(4*data.w*data.h)) == NULL) {
		fclose(fp);
		return imgloadBAD(fname);
	}

	fread(data.img_data, data.w*data.h, 4, fp);
	fclose(fp);

	return data;
}


IMGLOAD_DATA imgloadPNG(const char *fname) {
	IMGLOAD_DATA data;
	void **row_pointers;
	FILE *fp;
	int  i, row, bpp;
	png_structp png_ptr;
	png_infop info_ptr;

	fp = fopen(fname, "rb");

	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL) {
		fclose(fp);
		return imgloadBAD(fname);
	}

	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
		fclose(fp);
		png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL);
		return imgloadBAD(fname);
	}

	if (setjmp(png_ptr->jmpbuf)) {
		fclose(fp);
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
		return imgloadBAD(fname);
	}

	png_init_io(png_ptr, fp);
	png_read_info(png_ptr, info_ptr);
	png_set_sig_bytes(png_ptr, 8);
	png_get_IHDR(png_ptr, info_ptr, (void *) &data.w, (void *) &data.h, &bpp, &i, NULL, NULL, NULL);
	png_set_strip_16(png_ptr);
	png_set_expand(png_ptr);
	
	row_pointers = malloc(sizeof(void *) * data.h);

	for (i = 0; i < data.h; i++)
		row_pointers[i] = malloc(png_get_rowbytes(png_ptr, info_ptr));
	
	png_read_image(png_ptr, (png_bytepp) row_pointers);
	data.img_data = malloc(data.w * data.h * 4);
	row = png_get_rowbytes(png_ptr, info_ptr);
	
	for (i = 0; i < data.h; i++) {
		memcpy(&data.img_data[i*(row/4)], row_pointers[i], row);
		free(row_pointers[i]);
	}

	png_read_end(png_ptr, info_ptr);
	png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
	fclose(fp);

	return data;
}


IMGLOAD_DATA imgloadLoad(const char *fname) {
	int res;

	res = imgloadCheckFormat(fname);

	switch (res) {
		case IMG_IS_PNG:
			return imgloadPNG(fname);
			break;
		case IMG_IS_RAW:
			return imgloadRAW(fname);
			break;
		default:
			return imgloadBAD(fname);
			break;
	}
}
