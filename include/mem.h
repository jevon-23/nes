#include "utils.h"
#include <inttypes.h>
#include <stdbool.h>

/* Memory Map for the NES system
 *
 * - NES uses 16-bit memory addressing -> can access 2^16 bits
 *   == 65535
 *
 * - memory is divided up into 5 parts, shown in base and bound section
 */

#ifndef MEM_H
#define MEM_H

typedef struct memory {
    char mem[65536]; /* 16-bit mem addressing = 65536 bytes*/
} memory;

/* == base and bounds == */

/* ram */
#define ram_start 0
#define stack_start 0x100 /* stack pointer start */
#define stack_end 0x1FF
#define ram_end 0x2000

/* redirect to other hardware modules */
#define redir_start 0x2000
#define redir_end 0x4020

/* special space for some cartriges (ignored) */
#define special_cart1_start 0x4020
#define special_cart1_end 0x6000

/* ram space on a cartridge. (ignored) */
#define cart_ram_start 0x6000
#define cart_ram_end 0x8000

/* program rom space on cartridge */
#define cart_prgrom_start 0x8000
#define cart_prgrom_end 0xffff // note: not inclusive

/* functions */

/* is this address within the rams' bounds */
bool valid_ram_bounds(uint16_t address);

/* is this address within the stack bounds */
bool valid_stack_bounds(uint16_t address);

/* is this address within the redirection bounds */
bool valid_redir_bounds(uint16_t address);

/* is this address within the program rom bounds */
bool valid_prgrom_bounds(uint16_t address);

/* create a new instance of memory */
memory *init_memory();

/* free memory struct */
void free_memory(memory *mem);

/* write the rom file for the game into memory */
void write_game_file(memory *mem, aFile gameFile);

#endif
