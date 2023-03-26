#include "stego.h"
#include "list.h"
#include "bmp.h"
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdio.h>
#include <stddef.h>

int get_number(char c) {
  char symb[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R',
'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', ' ', '.', ','};
  for (int i = 0; i < 29; i++)
    if (symb[i] == c)
      return i;
  return -1;
}

char get_symbol(int n) {
  char symb[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R',
'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', ' ', '.', ','};
  return symb[n];
}

char* reverse_bin(int i) {
    size_t bits = sizeof(char) * 5;

    char* str = malloc(6);
    if(!str) return NULL;
    str[bits] = 0;

    unsigned u = *(unsigned *)&i;
    for(; bits--; u >>= 1)
        str[bits] = u & 1 ? '1' : '0';

    for (int i = 0; i < (int)strlen(str) / 2; i++)
    {
      char ch = str[i];
      str[i] = str[strlen(str) - i - 1];
      str[strlen(str) - i - 1] = ch;
    }

    return str;
}

int add_pixel(struct intrusive_list* list, int x, int y, char c) {
  struct pixel* p = malloc(sizeof(*p));
  if (p == NULL)
    return 1;
  p->x = x;
  p->y = y;
  p->color = c;
  add_node(list, &(p->node));
  return 0;
}

int readKey(FILE* file, struct intrusive_list* list) {
	char line[256];
  while (fgets(line, sizeof(line), file)) {
      char * pch;
      pch = strtok (line," \n");
      int x = atoi(pch);
      pch = strtok (NULL, " ");
      int y = atoi(pch);
      pch = strtok (NULL, " ");
      char c = *pch;
      if (add_pixel(list, x, y, c))
        return 1;
  }
  return 0;
}

void readMessage(FILE* file, char* message) {
  fgets(message, 1024, file);
  message[strcspn(message, "\n")] = 0;
}

int saveImage(struct Image* image, struct intrusive_list* pixels, char* message) {
  if ((int)strlen(message) * 5 > get_length(pixels))
    return 1;
  for (int i = 0; i < (int)strlen(message); i++) {
    int code;
    code = get_number(message[i]);
    if (code == -1)
      return 1;
    char* str = reverse_bin(code);
    for (int j = i*5; j < i*5 + 5; j++) {
      if (j < get_length(pixels)) {
        struct pixel* pixel = container_of(get_node_by_index(pixels, j), struct pixel, node);
        if (pixel->color == 'R') {
          if (strlen(str) > (size_t)(j - i*5))
            image->rgb[pixel->y][pixel->x].red = image->rgb[pixel->y][pixel->x].red / 2 * 2 + (str[j - i*5] - '0');
          else
            image->rgb[pixel->y][pixel->x].red = image->rgb[pixel->y][pixel->x].red / 2 * 2 + 0;
        }
        if (pixel->color == 'G') {
          if (strlen(str) > (size_t)(j - i*5))
            image->rgb[pixel->y][pixel->x].green = image->rgb[pixel->y][pixel->x].green / 2 * 2 + (str[j - i*5] - '0');
          else
            image->rgb[pixel->y][pixel->x].green = image->rgb[pixel->y][pixel->x].green / 2 * 2 + 0;
        }
        if (pixel->color == 'B') {
          if (strlen(str) > (size_t)(j - i*5))
            image->rgb[pixel->y][pixel->x].blue = image->rgb[pixel->y][pixel->x].blue / 2 * 2 + (str[j - i*5] - '0');
          else
            image->rgb[pixel->y][pixel->x].blue = image->rgb[pixel->y][pixel->x].blue / 2 * 2 + 0;
        }
      }
    }
    free(str); 
  }
  return 0;
}

void extractMessage(struct Image* image, struct intrusive_list* pixels, char* message) {
  for (int i = 0; i < get_length(pixels); i+=5) {
    int res = 0;
    for (int j = i + 4; j >= i; j--) {
      if (j < get_length(pixels)) {
        struct pixel* pixel = container_of(get_node_by_index(pixels, j), struct pixel, node);
        if (pixel->color == 'R') {
          res = res * 2 + (image->rgb[pixel->y][pixel->x].red % 2);
        }
        if (pixel->color == 'G') {
          res = res * 2 + (image->rgb[pixel->y][pixel->x].green % 2);
        }
        if (pixel->color == 'B') {
          res = res * 2 + (image->rgb[pixel->y][pixel->x].blue % 2);
        }
      }
    }
    message[i/5] = get_symbol(res);
  }
}

void saveMessage(FILE* file, char* message) {
  fputs(message, file);
  fputc('\n', file);
}