///////////////////////////////////////////////////////////////////////////////
////////////////// Peter Stamos CS449 Project 1: bmp_edit.c ///////////////////
///////////////////////////////////////////////////////////////////////////////

// Included libraries...
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Included packs...
#pragma pack(1)

///////////////////////////////////////////////////////////////////////////////
////////////////////////////////// STRUCTS ////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

typedef struct BMP {
  unsigned short format_ID;
  unsigned int file_size;
  unsigned short reserved_1;
  unsigned short reserved_2;
  unsigned int offset;
} BMP;

/////////////////////////////

typedef struct DIB {
  unsigned int header_size;
  int width;
  int height;
  unsigned short planes;
  unsigned short bits;
  unsigned int compression;
  unsigned int image_size;
  int horizontal;
  int vertical;
  unsigned int pallete;
  unsigned int colors;
} DIB;

/////////////////////////////

typedef struct PIXELS {
  unsigned char blue;
  unsigned char green;
  unsigned char red;
} PIXELS;

///////////////////////////////////////////////////////////////////////////////
//////////////////////////////////// MAIN /////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[]) {

  FILE *input;
  FILE *output;
  BMP header;
  DIB information;
  PIXELS colors;
  char mode;
  int height;
  int padding;
  int i;
  int j;
  int h;
  float y;

  // Determine command-line argument for switch statement...
  if(strcmp(argv[1],"-invert") == 0) {
    mode = 1;
  }
  else if(strcmp(argv[1],"-grayscale") == 0) {
    mode = 2;
  }
  else {
    printf("Command-Line Input Error...\n");
    exit(1);
  }

  // Open image to read data...
  input = fopen(argv[2], "r");

  // If error occurs while opening file, terminate program...
  if(input == NULL) {
    printf("Error Opening Image...\n");
    exit(1);
  }

  // Read BMP header and DIB information from input file...
  fread(&header, sizeof(BMP), 1, input);
  fread(&information, sizeof(DIB), 1, input);

  // Ensure input file is .BMP format...
  if(header.format_ID != 0x4d42)  {
    printf("File Format Error...\n");
    fclose(input);
    fclose(output);
    exit(1);
  }

  // Ensure input file is 24-bits...
  if(information.bits != 24)  {
    printf("File Format Error...\n");
    fclose(input);
    fclose(output);
    exit(1);
  }

  // Switch statement will direct the command-line argument to the
  // specified image modification...
  switch (mode) {

    // Creates inverted image...
    case 1:

      // Create output file to write color modifications to...
      output = fopen("inverted.bmp", "w");

      // If error occurs while creating file, terminate program...
      if(output == NULL) {
        printf("Error creating inverted image...\n");
        exit(1);
      }

      // Write BMP header and DIB information to output file...
      fwrite(&header, sizeof(BMP), 1, output);
      fwrite(&information, sizeof(DIB), 1, output);

      padding = (4 - (information.width * sizeof(PIXELS)) % 4) % 4;

      for (i = 0, height = abs(information.height); i < height; i++) {
        for (j = 0; j < information.width; j++) {

          // Read pixel data from input file...
          fread(&colors, sizeof(PIXELS), 1, input);

          // Bitwise-NOT the RGB colors to give inverted appearance...
          colors.blue = ~colors.blue;
          colors.green = ~colors.green;
          colors.red = ~colors.red;

          // Write modified pixel data to output file...
          fwrite(&colors, sizeof(PIXELS), 1, output);
        }

        fseek(input, padding, SEEK_CUR);

        for (h = 0; h < padding; h++) {
          fputc(0x00, output);
        }
      }

      break;

///////////////////////////////////////////////////////////////////////////////

    // Creates grayscale image...
    case 2:

      // Create output file to write color modifications to...
      output = fopen("grayscale.bmp", "w");

      // If error occurs while creating file, terminate program...
      if(output == NULL) {
        printf("Error creating grayscale image...\n");
        exit(1);
      }

      // Write BMP header and DIB information to output file...
      fwrite(&header, sizeof(BMP), 1, output);
      fwrite(&information, sizeof(DIB), 1, output);

      padding = (4 - (information.width * sizeof(PIXELS)) % 4) % 4;

      for (i = 0, height = abs(information.height); i < height; i++) {
        for (j = 0; j < information.width; j++) {

          // Read pixel data from input file...
          fread(&colors, sizeof(PIXELS), 1, input);

          // Set RGB colors to y formula for grayscale appearance...
          y = 0.2126 * colors.red + 0.7152 * colors.green + 0.0722 * colors.blue;
          colors.blue = (char)y;
          colors.green = (char)y;
          colors.red = (char)y;

          // Write modified pixel data to output file...
          fwrite(&colors, sizeof(PIXELS), 1, output);
        }

        fseek(input, padding, SEEK_CUR);

        for (h = 0; h < padding; h++) {
          fputc(0x00, output);
        }
      }

      break;

///////////////////////////////////////////////////////////////////////////////

    // Catch command-line arguments that are invalid...
    default:

      printf("Unrecognized Command-Line Entry...\n");
      exit(1);
  }

  // Close input and output files...
  fclose(input);
  fclose(output);

  return 0;
}

///////////////////////////////////////////////////////////////////////////////
////////////////// Peter Stamos CS449 Project 1: bmp_edit.c ///////////////////
///////////////////////////////////////////////////////////////////////////////
