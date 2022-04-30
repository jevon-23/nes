#include "../../include/utils.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX_PROGRAM_LEN 500000000

/* convert 16 bits from big endian into little endian format
 * a = 1100 0011 1010 1111
 * a << 8 -> 1010 1111
 *
 * hi = 1010 1111 0000 0000
 * lo = 0000 0000 1100 1111
 * h|l= 1010 1111 1100 1111
 */
uint16_t switch_endian(uint16_t bytes) {
  uint16_t new_high = bytes << 8;
  uint16_t new_low = bytes >> 8;
  return new_high | new_low;
}

/* convert 16 bits from lil endian 2 big endian format */
// uint16_t lil2big_endian(uint16_t bytes) {
//   uint16_t new_high = bytes << ;
//   uint16_t new_low = 0;
//   return new_high | new_low;
// }

// If we run into an error, return a message and exit program.
void error(int code) {
  switch (code) {
  case 0: // Invalid .extension
    printf("Invalid file type. We want this to work for .py, .java, .c, and "
           ".golang sometime in the future, but a sof rn only .c files are "
           "available");
  default:
    printf("Something went wrong");
  }
  exit(-1);
}

// Print a seperator
void printSpace() {
  printf("\n~~~~~~~~~~~~~~~~~~~~~~~~");
  printf("\n");
}

/*
Reads the contents of theFile.
*/
void readContents(aFile theFile) {
  printf("printing %d chars\n", theFile.fileSize);
  for (int i = 0; i < theFile.fileSize; i++) {
    printf("char %d = %x\n", i, *(*theFile.contents + i));
  }
}

/*
Reads the file, stores the name and its contents inside a AFILE struct
*/
aFile readFile(char *fileName) {
  printf("fileName = %s\n", fileName);
  // Opening the file that we are reading
  FILE *currFile = fopen(fileName, "rb");
  if (currFile == NULL) {
    error(1);
  }

  // Creating a new afile instance. Making room for the first line to be read
  // from file
  aFile theFile;
  theFile.name = fileName;
  theFile.contents = (unsigned char **)malloc(
      sizeof(unsigned char *)); // The content that fileName holds
  *theFile.contents =
      (unsigned char *)malloc((sizeof(unsigned char) * MAX_PROGRAM_LEN));
  // Read this line from the file, and storing it at a value inside of
  // theContents
  theFile.fileSize = fread(*theFile.contents, sizeof(unsigned char),
                           MAX_PROGRAM_LEN, currFile);
  fclose(currFile);
  printf("read %s\n", fileName);
  return theFile;
}
