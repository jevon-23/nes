#include "mem.h"
#ifndef CPU_H
#define CPU_H

/* == structs == */

/* CPU Registers:
 * The registers that the cpu will be interacting with 
 * source: nesdev.org/wiki/CPU_registers 
 */
typedef struct cpu_registers {
    uint8_t A; /* accumulator; interacts with ALU; 1 byte */
    uint8_t X; /* used for addressing modes */
    uint8_t Y; /* used for addrsesing modes */
    uint16_t PC; /* program counter */
    uint8_t esp; /* stack pointer */
    uint8_t stat; /* stat register */
} cpu_registers;

/* CPU:
 * Struct for an instance of our cpu. CPU has access to 2
 * components, the memory map and the registers it interacts
 * with directly
 */
typedef struct cpu {
    memory *storage;
    cpu_registers *regs;
} cpu;

/* == CPU functions == */

/* initialize a new cpu instance */
cpu *init_cpu();

/* run fetch, decode, exec cycle */
void run_cpu(cpu *core);

/* handles the action associated w/ opcode */
int opcode_handler(cpu *core, uint8_t opcode);

/* free the cpu */
void free_cpu(cpu *core);

/* == Memory Access == */

/* read back a byte from memory at location ADDY */
uint8_t mem_read(cpu *core, uint16_t addy);

/* write DATA into memory @ ADDY */
void mem_write(cpu *core, uint16_t addy, uint8_t data);
#endif
