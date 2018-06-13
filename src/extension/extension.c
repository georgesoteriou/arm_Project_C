#include <stdlib.h>
#include <stdio.h>
#include <png.h>
#include <unistd.h>
#include <string.h>

#define BKG_R 187
#define BKG_G 70
#define BKG_B 226

#define SEP_R 0
#define SEP_G 255
#define SEP_B 0

#define NOTE_R 255
#define NOTE_G 150
#define NOTE_B 0

int width, height;
int startX, startY;
int endX, endY;
png_byte color_type;
png_byte bit_depth;
png_bytep *row_pointers;



void read_png_file(char *filename) {
 FILE *fp = fopen(filename, "rb");

  png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if(!png) abort();

  png_infop info = png_create_info_struct(png);
  if(!info) abort();

  if(setjmp(png_jmpbuf(png))) abort();

  png_init_io(png, fp);

  png_read_info(png, info);

  width      = png_get_image_width(png, info);
  height     = png_get_image_height(png, info);
  color_type = png_get_color_type(png, info);
  bit_depth  = png_get_bit_depth(png, info);

  // Read any color_type into 8bit depth, RGBA format.
  // See http://www.libpng.org/pub/png/libpng-manual.txt

  if(bit_depth == 16)
    png_set_strip_16(png);

  if(color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_palette_to_rgb(png);

  // PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
  if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
    png_set_expand_gray_1_2_4_to_8(png);

  if(png_get_valid(png, info, PNG_INFO_tRNS))
    png_set_tRNS_to_alpha(png);

  // These color_type don't have an alpha channel then fill it with 0xff.
  if(color_type == PNG_COLOR_TYPE_RGB ||
     color_type == PNG_COLOR_TYPE_GRAY ||
     color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

  if(color_type == PNG_COLOR_TYPE_GRAY ||
     color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
    png_set_gray_to_rgb(png);

  png_read_update_info(png, info);

  row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
  for(int y = 0; y < height; y++) {
    row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png,info));
  }

  png_read_image(png, row_pointers);
  fclose(fp);

}

//check if pixel is background color
int checkBKG(png_bytep px) {
  if(px[0] == BKG_R &&
    px[1] == BKG_G && 
    px[2] == BKG_B) {
    return 1;
  } else {
    return 0;
  }
}

int checkBlack(png_bytep px) {
  if(px[0] == 0 && px[1] == 0 && px[2] == 0) {
    return 1;
  } else {
    return 0;
  }
}

//ceck if fixel is background color
int checkSEP(png_bytep px) {
  if(px[0] == SEP_R &&
    px[1] == SEP_G && 
    px[2] == SEP_B ) {
    return 1;
  } else {
    return 0;
  }
}


//ceck if fixel is background color
int checkNOTE(png_bytep px) {
  if(px[0] >= NOTE_R - 2 && px[0] <= NOTE_R &&
    px[1] >= NOTE_G - 2 && px[1] <= NOTE_G + 2 && 
    px[2] >= NOTE_B && px[2] <= NOTE_B + 2) {
    return 1;
  } else {
    return 0;
  }
}

//find start of notes
void findStart() {
  for(int y = 0; y < height; y++) {
    png_bytep row = row_pointers[y];
    for(int x = 0; x < width; x++) {
      png_bytep px = &(row[x * 4]);
      if(checkBKG(px) == 0 && checkBlack(px) == 0) { 
       startX = x;
       startY = y;
       return;
      }
    }
  }
}

//find the end of notes
void findEnd() {
  for(int y = height - 1; y >= 0; y--) {
    png_bytep row = row_pointers[y];
    for(int x = width - 1; x >= 0; x--) {
      png_bytep px = &(row[x * 4]);
      if(checkBKG(px) == 0  && checkBlack(px) == 0) {
        endX = x;
        endY = y;
        return;
      }
    }
  }
}

char* note_table[36] = {
  " pl C3",
  " pl C#3",
  " pl D3",
  " pl D#3",
  " pl E3",
  " pl F3",
  " pl F#3",
  " pl G3",
  " pl G#3",
  " pl A3",
  " pl A#3",
  " pl B3",
  " pl C4",
  " pl C#4",
  " pl D4",
  " pl D#4",
  " pl E4",
  " pl F4",
  " pl F#4",
  " pl G4",
  " pl G#4",
  " pl A4",
  " pl A#4",
  " pl B4",
  " pl C5",
  " pl C#5",
  " pl D5",
  " pl D#5",
  " pl E5",
  " pl F5",
  " pl F#5",
  " pl G5",
  " pl G#5",
  " pl A5",
  " pl A#5",
  " pl B5"
};


void process_png_file() {
   /* for(int y = startY; y < startY + 100; y++ ) {
      png_bytep row = row_pointers[y];
      for(int x = startX; x < startX + 50; x++) {
        png_bytep px = &(row[x * 4]);

        if(px[0] == 187 && px[1] == 70 && px[2] == 226) {
          printf("p");
        } else if(px[0] == 255 && px[1] == 255 && px[2] == 255) {
          printf("w");
        } else if(px[0] == 0 && px[1] == 255 && px[2] == 0) {
          printf("g");
        } else if(px[0] == 255 && px[1] == 150 && px[2] == 0) {
          printf("o");
        } else if(px[0] == 0 && px[1] == 0 && px[2] == 0){
          printf("b");
        }
      }

      printf("\n");
    }*/
  
  findStart();
  findEnd();

  for(int y = startY+1; y <= endY; y++) {
    png_bytep row = row_pointers[y];
    
    char* rowStr = calloc(sizeof(char), 235); 
    strcat(rowStr, "play -q -n synth 1");
    
    int notes = 0;
    for(int x = startX; x <= endX; x++) {
      png_bytep px = &(row[x * 4]);
      //gets rid of green
      //and increases the notes counter
      if(checkSEP(px) == 1) {
        notes++;
        while(checkSEP(px) == 1) {
         x++;
         px = &(row[x * 4]);
        }
      }

      if(checkNOTE(px) == 1) {
        strcat(rowStr, note_table[notes]);

        while(checkNOTE(px) == 1) {
         x++;
         px = &(row[x * 4]);
        }
      }
    }
    if(strlen(rowStr) > 18) {
      printf("%s\n", rowStr);
      system(rowStr);
    }else{
      sleep(1);
    }
    free(rowStr);

    png_bytep px = &(row[startX * 4]);

    while(checkSEP(px) == 0 && checkBKG(px) == 0) {
      y++;
      png_bytep row = row_pointers[y];
      px = &(row[startX * 4]);
    }

    while(checkSEP(px) == 1) {
      y++;
      png_bytep row = row_pointers[y];
      px = &(row[startX * 4]);
    }
  }
}


int main(int argc, char *argv[]) {
  if(argc != 2) abort();

  read_png_file(argv[1]);
  process_png_file();

  return 0;
}
