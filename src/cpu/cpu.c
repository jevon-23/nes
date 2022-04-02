#include "../../include/cpu.h"
#include "../../include/instruction_set.h"
#include "../../include/mem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* == memory i/o == */

/* write a value into memory */
void mem_write(cpu *core, uint16_t addy, uint8_t data) {
  memcpy(((core->storage->mem) + addy), &data, 1);
}

/* read a value from memory */
uint8_t mem_read(cpu *core, uint16_t addy) {
  uint8_t out = (uint8_t)(*(core->storage->mem) + addy);
  return out;
}

/* == init and destroy cpu == */

/* initialize cpu */
cpu *init_cpu() {
  cpu *core = (cpu *)malloc(sizeof(cpu));

  core->storage = init_memory(); /* init memory */
  core->regs = (cpu_registers *)calloc(sizeof(cpu_registers), 1); /*init regs*/
  core->regs->PC = cart_prgrom_start; /* set program counter */
  return core;
}

/* free this cpu */
void free_cpu(cpu *core) {
  free_memory(core->storage); /* free memory */
  free(core->regs);           /* free registers */
  free(core);                 /* free core */
}

/* == cpu functionality == */

/* reset the cpu state */
void reset_cpu(cpu *core) {
  core->regs->A = 0;
  core->regs->X = 0;
  core->regs->Y = 0;
  core->regs->PC = mem_read(core, 0xFFFC);
  core->regs->esp = 0;
  core->regs->stat = 0;
}
/* writes byte DATA into ADDY */
void write_byte(cpu *core, uint16_t addy, uint8_t data) {
  memcpy((core->storage->mem + addy), &data, 2);
}

/* read a byte from the address passed in */
uint8_t read_byte(cpu *core, uint16_t addy) {
  return *(core->storage->mem + addy);
}

/* Fetch the next instruction and increase the program counter */
uint8_t fetch_instruction(cpu *core) {
  uint8_t out = read_byte(core, core->regs->PC);
  core->regs->PC++;
  return out;
}

/* decode the instruction, then executes the functionality based on opcode */
int opcode_handler(cpu *core, uint8_t opcode) {
  uint8_t new_opcode;
  switch (opcode) {
  case 0x00:
    /* BRK */
    return 23; // jordan year baby

  case 0xAA:
    /* TAX */
    tax(core, opcode);
    break;

  /* LDA immediate */
  case 0xA9:;
    new_opcode = fetch_instruction(core);
    lda(core, new_opcode);
    break;

  /* CPY immediate */
  case 0xc0:;
    new_opcode = fetch_instruction(core);
    cpy(core, new_opcode);
    break;
  /* INC X */
  case 0xe8:
    inx(core, opcode);
    break;
  default:
    printf("Invalid opcode %x\n", opcode);
    // perror("Invalid opcode \n");
    exit(-1);
  }
  return 0;
}

/* run the cpu */
/* FIXME: inside of main.c, update to load a program into start place */
void run_cpu(cpu *core) {

  /* set ref point */
  mem_write(core, 0xFFFC, (uint8_t)cart_prgrom_start);
  int done = 0;
  while (!done) {
    uint8_t next_instruction = fetch_instruction(core);
    done = opcode_handler(core, next_instruction);
  }
}
