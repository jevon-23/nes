#include "../../include/cpu.h"
#include "../../include/mem.h"
#include <float.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* FIXME: update to load a program into start place */

int processCLI(int argc, char **argv, char **filename) {
  if (argc != 2) {
    printf("usage: ./chip8 <rom/filename>");
    exit(-1);
  }
  *filename = (char *)malloc(sizeof(char *) * strlen(*(argv + 1)));
  strcpy(*filename, *(argv + 1));
  return 0;
}

int main(int arg, char *argv[]) {
  char *filename;
  processCLI(arg, argv, &filename);
  aFile gameFile = readFile(filename);
  memory *mem = init_memory();
  write_game_file(&mem, gameFile);
  cpu *core = init_cpu();
  // display *screen = makeDisplay(); help w/ sdl i think
  // printf("printing out the memory real quick\n");
  // for (int i = 0; i < ramSize; i++) {
  //   printf("mem + %d = %x\n", i, *(memory->RAM + i));
  // }
  printf("Successful build! Running cpu!\n");
  run_cpu(core);
  //   buildWindow();

  return 0;
}
