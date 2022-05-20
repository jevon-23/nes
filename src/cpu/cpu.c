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

/* Write 2 bytes to memory */
void mem_write_16(cpu *core, uint16_t addy, uint16_t data) {
  uint16_t high = data >> 8;
  uint16_t low = (data & 0xff);
  mem_write(core, addy, low);
  mem_write(core, addy + 1, high);
}

/* read a value from memory */
uint8_t mem_read(cpu *core, uint16_t addy) {
  return *(core->storage->mem + addy);
}

/* Read 16 bits back */
uint16_t mem_read_16(cpu *core, uint16_t addy) {
  /* Get the lower 8 bits */
  uint8_t lo = mem_read(core, addy);
  uint16_t low = (uint16_t)lo;

  /* Get the upper 8 bits */
  uint8_t hi = mem_read(core, addy + 1);
  uint16_t high = (uint16_t)hi;

  return (high << 8) | low;
}
/* == init and destroy cpu == */

/* initialize cpu */
cpu *init_cpu() {
  cpu *core = (cpu *)malloc(sizeof(cpu));

  core->storage = init_memory(); /* init memory */
  core->regs = (cpu_registers *)calloc(sizeof(cpu_registers), 1); /*init regs*/
  core->regs->PC = 0xFFFC; /* set program counter */
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
  core->regs->PC = mem_read_16(core, 0xFFFC);
  core->regs->esp = 0;
  core->regs->stat = 0;
}

/* Fetch the next instruction and increase the program counter */
uint8_t fetch_instruction(cpu *core) {
  uint8_t out = mem_read(core, core->regs->PC);
  core->regs->PC++;
  return out;
}

/* decode the instruction, then executes the functionality based on opcode */
int opcode_handler(cpu *core, uint8_t opcode) {
  switch (opcode) {
  case 0x00:
    /* BRK */
    return 23; // jordan year baby

  case 0xAA:
    /* TAX */
    tax(core);
    break;

  /* STA Zero-page */
  case 0x85:
    sta(core, ZeroPage);
    break;
  /* STA Zero-page, x */
  case 0x95:
    sta(core, ZeroPage_X);
    break;
  /* STA Absolute */
  case 0x8d:
    sta(core, Absolute);
    break;
  /* STA Absolute, x */
  case 0x9d:
    sta(core, Absolute_X);
    break;
  /* STA Absolute, y */
  case 0x99:
    sta(core, Absolute_Y);
    break;
  /* STA Indirect, x */
  case 0x81:
    sta(core, Indirect_X);
    break;
  /* STA Indirect, y */
  case 0x91:
    sta(core, Indirect_Y);
    break;

  /* LDY Immediate */
  case 0xA0:
    ldy(core, Immediate);
    break;
  /* Zero-page */
  case 0xA4:
    ldy(core, ZeroPage);
    break;
  /* Zero-page, x */
  case 0xB4:
    ldy(core, ZeroPage_X);
    break;
  /* Absolute */
  case 0xAC:
    ldy(core, Absolute);
    break;
  /* Absolute, x */
  case 0xBC:
    ldy(core, Absolute_X);
    break;

  /* LDX Immediate */
  case 0xA2:
    ldx(core, Immediate);
    break;
  /* Zero-page */
  case 0xA6:
    ldx(core, ZeroPage);
    break;
  /* Zero-page, Y */
  case 0xB6:
    ldx(core, ZeroPage_Y);
    break;
  /* Absolute */
  case 0xAE:
    ldx(core, Absolute);
    break;
  /* Absolute */
  case 0xBE:
    ldx(core, Absolute_Y);
    break;

  /* LDA Immediate */
  case 0xA9:
    lda(core, Immediate);
    break;
  /* Zero page */
  case 0xA5:
    lda(core, ZeroPage);
    break;
  /* Zero page, x */
  case 0xB5:
    lda(core, ZeroPage_X);
    break;
  /* Absolute */
  case 0xAD:
    lda(core, Absolute);
    break;
  /* Absolute, X */
  case 0xBD:
    lda(core, Absolute_X);
    break;
  /* Absolute, Y */
  case 0xB9:
    lda(core, Absolute_Y);
    break;
  /* Indirect, X */
  case 0xA1:
    lda(core, Indirect_X);
    break;
  /* Indrect, Y */
  case 0xB1:
    lda(core, Indirect_Y);
    break;

  /* CPY immediate */
  case 0xc0:;
    cpy(core, Immediate);
    break;
  /* INC X */
  case 0xe8:
    inx(core);
    break;
  default:
    printf("Invalid opcode %x\n", opcode);
    exit(-1);
  }
  return 0;
}

/* run the cpu */
/* FIXME: inside of main.c, update to load a program into start place */
void run_cpu(cpu *core) {

  /* set ref point */
  mem_write_16(core, 0xFFFC, cart_prgrom_start);
  reset_cpu(core);
  int done = 0;
  while (!done) {
    uint8_t next_instruction = fetch_instruction(core);
    done = opcode_handler(core, next_instruction);
  }
}
