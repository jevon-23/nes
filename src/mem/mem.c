#include "../../include/mem.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* == init and destroy mem == */

/* allocate space for a mem structure */
memory *init_memory() {
  memory *out = (memory *)malloc(sizeof(memory));
  return out;
}

/* frees the space allocated for a mem structure */
void free_memory(memory *mem) { free(mem); }

/* == bounds checking for memory access == */

/* helper comparator for abstraction */
bool check_bounds(uint16_t address, uint16_t lower, uint16_t upper) {
  if (address < lower || address > upper) {
    return false;
  }
  return true;
}

/* check if ADDRESS is within the ram bounds */
bool valid_ram_bounds(uint16_t address) {
  return check_bounds(address, ram_start, ram_end);
}

/* check if ADDRESS is within the stack bounds */
bool valid_stack_bounds(uint16_t address) {
  return check_bounds(address, stack_start, stack_end);
}

/* check if ADDRESS is within the redir bounds */
bool valid_redir_bounds(uint16_t address) {
  return check_bounds(address, redir_start, redir_end);
}

/* check if ADDRESS is within the prgrom bounds */
bool valid_prgrom_bounds(uint16_t address) {
  return check_bounds(address, cart_prgrom_start, cart_prgrom_end);
}

/* write a game file into memory */
void writeGameFile(memory **mem, aFile gameFile) {
  memcpy(((*mem)->mem + (cart_prgrom_start)), *gameFile.contents,
         gameFile.fileSize);
}
