#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmp.h"
#include "list.h"
#include "stego.h"
#include <stddef.h>

#define open_command_position 1
#define input_file_path_position 2
#define output_file_path_position 3
#define key_file_path_position_insert 4
#define message_file_path_position_insert 5
#define key_file_path_position_extract 3
#define message_file_path_position_extract 4

int main(int argc, char *argv[]) {

  if ((argc == 8) && (strcmp(argv[open_command_position],"crop-rotate") == 0)) {

    FILE *Ifile = fopen(argv[input_file_path_position], "rb");

    struct BMP_header* header = (struct BMP_header*)malloc(sizeof(struct BMP_header));
    struct BITMAPINFO* bitmapinfo = (struct BITMAPINFO*)malloc(sizeof(struct BITMAPINFO));
    struct Image* image = (struct Image*)malloc(sizeof(struct Image));

    if ((Ifile != NULL) && (header != NULL) && (bitmapinfo != NULL) && (image != NULL)) {

      if (load_bmp(Ifile, header, bitmapinfo, image))
        return 1;
      
      int x = atoi(argv[4]);
      int y = atoi(argv[5]);
      int w = atoi(argv[6]);
      int h = atoi(argv[7]);


      if (
        (w < 0) || 
        (h < 0) || 
        (x < 0) || 
        (y < 0) || 
        (x + w > image->width) || 
        (y + h > image->height)
        ) {
        return 1;
      }

      if (crop(header, bitmapinfo, image, x, y, w, h) || rotate(bitmapinfo, image))
        return 1;


      FILE *Ofile = fopen(argv[output_file_path_position], "wb");

      if (Ofile == NULL)
        return 1;
  
      save_bmp (Ofile, header, bitmapinfo, image);
      

      fclose(Ofile);
      fclose(Ifile);
      free(header);
      free(bitmapinfo);
      free_image(image);
      free(image);

      return 0;
      
    }
    else {
      if (header != NULL)
        free(header);
      if (bitmapinfo != NULL)
        free(bitmapinfo);
      if (image != NULL)
        free(image);
    }

  }
  else if ((argc == 6) && (strcmp(argv[open_command_position],"insert") == 0)) {

    FILE *Ifile = fopen(argv[input_file_path_position], "rb");
    FILE *Ikey = fopen(argv[key_file_path_position_insert], "r");
    FILE *Imes = fopen(argv[message_file_path_position_insert], "r");

    struct BMP_header* header = (struct BMP_header*)malloc(sizeof(struct BMP_header));
    struct BITMAPINFO* bitmapinfo = (struct BITMAPINFO*)malloc(sizeof(struct BITMAPINFO));
    struct Image* image = (struct Image*)malloc(sizeof(struct Image));

    struct intrusive_list key;
    init_list(&key);

    char* message = (char*)calloc(1024, sizeof(char));

    if ((Ifile != NULL) && (Ikey != NULL) && (Imes != NULL) && (header != NULL) && (bitmapinfo != NULL) && (image != NULL) && (message != NULL)) {

      if (load_bmp(Ifile, header, bitmapinfo, image) || readKey(Ikey, &key))
        return 1;

      readMessage(Imes, message);
      if (saveImage(image, &key, message))
        return 1;
      FILE *Ofile = fopen(argv[output_file_path_position], "wb");

      if (Ofile == NULL)
        return 1;
      
      save_bmp (Ofile, header, bitmapinfo, image);
      

      fclose(Ofile);
      fclose(Ifile);
      fclose(Ikey);
      fclose(Imes);
      free(header);
      free(bitmapinfo);
      free_image(image);
      free(image);

      remove_all_pixels(&key);
      free(key.head);

      free(message);
      return 0;
      
    }
    else {

      if (header != NULL)
        free(header);
      if (bitmapinfo != NULL)
        free(bitmapinfo);
      if (image != NULL)
        free(image);
      if (message != NULL)
        free(message);
      remove_all_pixels(&key);
      free(key.head);

    }
  }
  else if ((argc == 5) && (strcmp(argv[open_command_position],"extract") == 0)) {

    FILE *Ifile = fopen(argv[input_file_path_position], "rb");
    FILE *Ikey = fopen(argv[key_file_path_position_extract], "r");
    struct BMP_header* header = (struct BMP_header*)malloc(sizeof(struct BMP_header));
    struct BITMAPINFO* bitmapinfo = (struct BITMAPINFO*)malloc(sizeof(struct BITMAPINFO));
    struct Image* image = (struct Image*)malloc(sizeof(struct Image));

    struct intrusive_list key;
    init_list(&key);

    char* message = (char*)calloc(1024, sizeof(char));

    if ((Ifile != NULL) && (Ikey != NULL) && (header != NULL) && (bitmapinfo != NULL) && (image != NULL) && (message != NULL)) {

      if (load_bmp(Ifile, header, bitmapinfo, image) || readKey(Ikey, &key))
        return 1;
      extractMessage(image, &key, message);


      FILE *Omes = fopen(argv[message_file_path_position_extract], "w");

      if (Omes == NULL)
        return 1;

      saveMessage(Omes, message);
      fclose(Ifile);
      fclose(Ikey);
      fclose(Omes);
      free(header);
      free(bitmapinfo);
      free_image(image);
      free(image);

      remove_all_pixels(&key);
      free(key.head);

      free(message);

      return 0;
      
    }
    else {

      if (header != NULL)
        free(header);
      if (bitmapinfo != NULL)
        free(bitmapinfo);
      if (image != NULL)
        free(image);
      if (message != NULL)
        free(message);
      remove_all_pixels(&key);
      free(key.head);

    }
  }
  return 1;
}
