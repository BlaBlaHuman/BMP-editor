#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"

void free_image(struct Image* image) {

	for (int i = 0; i < image->height; i++) {
		free(image->rgb[i]);
	}
	free(image->rgb);
	
}

int load_bmp(FILE* file, struct BMP_header* header, struct BITMAPINFO* bitmapinfo, struct Image* image) {

	fread(header, sizeof(struct BMP_header), 1, file);
	fread(bitmapinfo, sizeof(struct BITMAPINFO), 1, file);
	fseek(file, header->offset, SEEK_SET);


	image->width = bitmapinfo->width;
	image->height = bitmapinfo->height;

	image->rgb = (struct RGB**)malloc(image->height * sizeof(struct RGB*));
	if (image->rgb == NULL)
		return 1;

	char buffer[4];

	for (int i = image->height - 1; i >= 0; i--) {
		image->rgb[i] = (struct RGB*)malloc(image->width * sizeof(struct RGB));
		if (image->rgb[i] == NULL)
			return 1;
		fread(image->rgb[i], sizeof(struct RGB) * image->width, 1, file);
		fread(&buffer, (4 - bitmapinfo->width * 3 % 4) % 4, 1, file);
	}
	return 0;
}

int crop (struct BMP_header* header, struct BITMAPINFO* bitmapinfo, struct Image* image, int x, int y, int w, int h) {

	struct Image* copy = (struct Image*)malloc(sizeof(struct Image));

	if (copy == NULL)
		return 1;

	copy->width = w;
	copy->height = h;
	copy->rgb = (struct RGB**)malloc(h * sizeof(struct RGB*));

	if (copy->rgb == NULL)
 		return 1;

	for (int i = 0; i < copy->height; i++) {
		copy->rgb[i] = (struct RGB*)malloc(w * sizeof(struct RGB));
		if (copy->rgb[i] == NULL)
			return 1;
		for (int j = 0; j < copy->width; j++) {
			copy->rgb[i][j] = image->rgb[i + y][j + x];
		}
		
	}

	free_image(image);
	*image = *copy;
	free(copy);

	header->size = w * h * 3 + sizeof(struct BITMAPINFO) + sizeof(struct BMP_header);
	bitmapinfo->height = h;
	bitmapinfo->width = w;
	bitmapinfo->sizeImage = w * h * 3;

	return 0;
}

int rotate (struct BITMAPINFO* bitmapinfo, struct Image* image) {

    struct Image* copy = (struct Image*)malloc(sizeof(struct Image));

    if (copy == NULL)
    	return 1;
    
    copy->width = image->height;
    copy->height = image->width;
    copy->rgb = (struct RGB**)malloc(copy->height * sizeof(struct RGB*));

	if (copy->rgb == NULL)
		return 1;

	for (int i = 0; i < copy->height; i++) {
		copy->rgb[i] = (struct RGB*)malloc(copy->width * sizeof(struct RGB));
		if (copy->rgb[i] == NULL)
			return 1;
		for (int j = 0; j < copy->width; j++) {
			if (j >= image->height) {
				struct RGB tmp;
				tmp.red = 0;
				tmp.blue = 0;
				tmp.green = 0;
				copy->rgb[i][j] = tmp;
			}
			else {
				copy->rgb[i][j] = image->rgb[copy->width - j - 1][i];
			}
		}
	}

	free_image(image);
	*image = *copy;
	free(copy);

	bitmapinfo->height = image->height;
	bitmapinfo->width = image->width;
	
	return 0;
}

void save_bmp(FILE* file, struct BMP_header* header, struct BITMAPINFO* bitmapinfo, struct Image* image) {
	int number_of_zero_bytes = (4 - bitmapinfo->width * 3 % 4) % 4;
	header->size += number_of_zero_bytes * image->height;
	bitmapinfo->sizeImage = header->size - 54;
	fwrite(header, sizeof(struct BMP_header), 1, file);
	fwrite(bitmapinfo, sizeof(struct BITMAPINFO), 1, file);
	for (int i = image->height - 1; i >= 0; i--) {
		fwrite(image->rgb[i], sizeof(struct RGB) * image->width, 1, file);
		for (int k = 0; k < number_of_zero_bytes; k++)
			fputc(0, file);
	}	
}