#include <inttypes.h>
#ifndef UTILS_H
#define UTILS_H

/* NES cpu uses lil endian => we need a converter to handle that */

typedef struct aFile {
  char *name;      // Name of file
  unsigned char **contents; // Contents in file
  int fileSize;
} aFile;

uint16_t big2lil_endian(uint16_t bytes);
aFile readFile(char *fileName);
void readContents(aFile theFile);
void printSpace();
#endif 
