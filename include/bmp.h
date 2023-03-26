#ifndef BMP_H
#define BMP_H

#pragma pack(push,1)
struct BMP_header {
	char name[2];
	unsigned int size;
	int data;
	unsigned int offset;
};
#pragma pack(pop)

#pragma pack(push,1)
struct BITMAPINFO {
	int size;
	int width;
	int height;
	short planes;
	short bitCount;
	int compression;
	int sizeImage;
	int XPelsPerMeter;
	int YPelsPerMeter;
	int ClrUsed;
	int ClrImportant;
};
#pragma pack(pop)

#pragma pack(push,1)
struct RGB {
	unsigned char blue;
	unsigned char green;
	unsigned char red;
};
#pragma pack(pop)

struct Image {
	int width;
	int height;
	struct RGB **rgb;
};

void free_image(struct Image* image);
int rotate (struct BITMAPINFO* bitmapinfo, struct Image* image);
int crop (struct BMP_header* header, struct BITMAPINFO* bitmapinfo, struct Image* image, int x, int y, int w, int h);
int load_bmp(FILE* file, struct BMP_header* header, struct BITMAPINFO* bitmapinfo, struct Image* image);
void save_bmp(FILE* file, struct BMP_header* header, struct BITMAPINFO* bitmapinfo, struct Image* image);

#endif