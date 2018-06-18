#include <stdlib.h>
#include <stdio.h>
#include <png.h>
#include <unistd.h>
#include <string.h>


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

#define START_X 234
#define START_Y 38
#define NOTE_SIZE_X 80
#define NOTE_SIZE_Y 40
#define SPACE_SIZE_X 10
#define SPACE_SIZE_Y 7 
#define NOTES 36
#define TIMES 50
#define COLOR_B 118
#define WHITE_B 255
#define DURATION 0.1


void addNote(int note, double timeout, double duration, char* outfile) {
  // play -n -q synth "DURATION" pl C4 delay "TIMEOUT" | play -n -q synth 1 pl E4 delay 0
  char t[10], d[10], totalD[10];
  snprintf(t, 10, "%f", timeout);
  snprintf(d, 10, "%f", duration);
  snprintf(totalD, 10, "%f", DURATION * 100);
  char *toAdd = calloc(sizeof(char), 100);
 
  strcat(toAdd, "sox -n result-temp.wav synth ");
  strcat(toAdd, d);
  strcat(toAdd, note_table[note]);
  strcat(toAdd, " delay ");
  strcat(toAdd, t);
                                                          
  // printf("%s\n", toAdd);
  // create new note
  system(toAdd);
  free(toAdd);

  char *result = calloc(sizeof(char), 100);
  strcat(result, "sox -V0 -m -v 1 ");
  strcat(result, outfile);
  strcat(result, " -v 1 result-temp.wav out.wav trim 0 ");
  strcat(result, totalD);
  // create mix of files
  system(result);
  free(result);
  
  char* moveOutput = calloc(sizeof(char), 100);
  strcat(moveOutput, "mv out.wav ");
  strcat(moveOutput, outfile);
  // move outfile
  system(moveOutput);
  free(moveOutput);

  system("rm result-temp.wav");
}


void process_png_file(char *outfile) {
  int duration = 0;
  int note = 0;
  for(int x = START_X; x <= (NOTES-1) * (NOTE_SIZE_X + SPACE_SIZE_X) + START_X ; x+= (NOTE_SIZE_X + SPACE_SIZE_X) , note++) {
    int timeout = 0;
    for(int y = START_Y; y <= (TIMES-1) * (NOTE_SIZE_Y + SPACE_SIZE_Y) + START_Y; y+= (NOTE_SIZE_Y + SPACE_SIZE_Y)) {
      png_bytep topLeftpx = &(row_pointers[y][x*4]);
      png_bytep midLeftpx = &(row_pointers[y+(NOTE_SIZE_Y/2)][x*4]);
      if(topLeftpx[2] == COLOR_B && midLeftpx[2] == COLOR_B){
        duration+= 2;
      }else if(topLeftpx[2] == COLOR_B && midLeftpx[2] == WHITE_B){
        duration++;
        //CREATE NEW NOTE
        //printf("new note with duration: %lf, Timeout: %lf\n", duration * DURATION, (timeout - duration + 1) * DURATION);
        addNote(note, ((timeout - duration + 1) * DURATION), duration * DURATION, outfile);
        duration = 0;
      }else if(topLeftpx[2] == WHITE_B && duration != 0){
        //CREATE NEW NOTE
        //printf("new note with duration: %lf, Timeout: %lf\n", duration * DURATION, (timeout - duration) * DURATION);
        addNote(note, ((timeout - duration) * DURATION), duration * DURATION, outfile);
        duration = 0;
      }
      if(midLeftpx[2] == COLOR_B && topLeftpx[2] != COLOR_B){
        duration++;
      }
      timeout+=2;
    }
  }
}


int main(int argc, char *argv[]) {
  if(argc != 3) abort();

  char totalD[10];  
  snprintf(totalD, 10, "%f", DURATION * 100);

  char *result = calloc(sizeof(char), 100); 
  strcat(result, "sox -n ");
  strcat(result, argv[2]);
  strcat(result, " trim 0 ");
  strcat(result, totalD);
  // Create result file
  system(result);
  free(result);

  read_png_file(argv[1]);
  process_png_file(argv[2]);
  
  char *play = calloc(sizeof(char), 100); 
  strcat(play, "play ");
  strcat(play, argv[2]);
  // play outfile
  system(play); 
  free(play);  

//printf("%s\n", result);

  for(int y = 0; y < height; y++) {
    free(row_pointers[y]);
  }
  free(row_pointers);

  return 0;
}
