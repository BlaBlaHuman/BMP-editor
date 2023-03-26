#ifndef STEGO_H
#define STEGO_H

#include <stdio.h>
#include "list.h"
#include "bmp.h"

void readMessage(FILE* file, char* message);
int readKey(FILE* file, struct intrusive_list* list);

int saveImage(struct Image* image, struct intrusive_list* pixels, char* message);

void extractMessage(struct Image* image, struct intrusive_list* pixels, char* message);
void saveMessage(FILE* file, char* message);


#endif