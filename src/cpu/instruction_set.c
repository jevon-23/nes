#include "../../include/cpu.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void set_zero_flag(cpu *core, uint8_t val);
void set_negative_flag(cpu *core, uint8_t val);
void set_carry_flag(cpu *core, uint8_t val, bool set);
size_t get_params(cpu *core, int mode);

/* just trying to understand the flags, notice that if both negative flag
 * and zero flag are not set, we have 0xff, else we don't
 */

/* === instructions === */

/* Clear carry flag => set status reg = 0 */
int clc(cpu *core) {
  core->regs->stat &= 0x00;
  return 0;
}

/* Set carry flag = 1 */
int sec(cpu *core) {
  core->regs->stat &= 0x01; /* set flag = 1 */
  return 0;
}

/* Loads DATA into REGS */
void load(cpu *core, size_t data, uint8_t *reg) {
  /* set zero flag */
  set_zero_flag(core, data);

  /* set negative flag */
  set_negative_flag(core, data);

  /* set the register */
  *reg = data;
}

/* Loads a byte of memory into the X register */
int ldy(cpu *core, int mode) {
  uint8_t instruction = get_params(core, mode);
  load(core, instruction, &core->regs->Y);
  return 0;
}

/* Loads a byte of memory into the X register */
int ldx(cpu *core, int mode) {
  uint8_t instruction = get_params(core, mode);
  load(core, instruction, &core->regs->X);
  return 0;
}

/* Loads a byte of memroy into the accumulator register */
int lda(cpu *core, int mode) {
  uint8_t instruction = get_params(core, mode);
  load(core, instruction, &core->regs->A);
  return 0;
}

/* Load the value in the A register to X register */
int tax(cpu *core) {
  uint8_t a = core->regs->A;

  /* set 0 flag */
  set_zero_flag(core, a);

  /* set negative flag */
  set_negative_flag(core, a);

  /* set the A register */
  core->regs->X = core->regs->A;

  return 0;
}

/* cpu->regs->x += 1 */
int inx(cpu *core) {
  /* inc. */
  core->regs->X++;
  uint8_t x = core->regs->X;

  /* set_zero flag */
  set_zero_flag(core, x);

  /* set negative flag */
  set_negative_flag(core, x);

  return 0;
}

/* compares the value to the Y register */
int cpy(cpu *core, int mode) {
  uint8_t instruction = get_params(core, mode);
  uint8_t y = core->regs->Y;

  /* set negative flag if bit is set in status register */
  set_negative_flag(core, core->regs->stat);

  /* compare y register with byte from program */
  if (y >= instruction) {
    set_carry_flag(core, y, true);
  } else if (y == 0) {
    set_zero_flag(core, 0);
  }

  return 0;
}

/* == Deal with flags == */

/* Sets the status register for the zero flag */
void set_zero_flag(cpu *core, uint8_t val) {
  (val == 0) ? (core->regs->stat |= 0x02) : (core->regs->stat &= 0xfd);
}

/* Sets the status register for the negative flag */
void set_negative_flag(cpu *core, uint8_t val) {
  (val & 0x80) ? (core->regs->stat |= 0x08) : (core->regs->stat &= 0x7f);
}

/* sets the carry flag */
void set_carry_flag(cpu *core, uint8_t val, bool set) {
  (set) ? sec(core) : clc(core);
}

/* Memory I/O */

/* Fetch the next instruction and increase the program counter */
uint8_t fetch_byte(cpu *core) {
  uint8_t out = mem_read(core, core->regs->PC);
  core->regs->PC++;
  return out;
}

/* Fetch the next instruction and increase the program counter */
uint16_t fetch_two_bytes(cpu *core) {
  uint16_t out = mem_read_16(core, core->regs->PC);
  core->regs->PC += 2;
  return out;
}

/* Wrap around inp + offset % 10000 */
uint16_t wrap_around_2byte_add(size_t inp, size_t offset) {
  return (inp + offset) % 0x10000;
}

/* Input + offset 256 (0x100) */
uint16_t wrap_around_byte_add(size_t inp, size_t offset) {
  return (uint16_t)((inp + offset) % 256);
}

/* Get parameters based on the addressing mode that was passed in */
size_t get_params(cpu *core, int mode) {
  size_t out = 0;
  uint8_t zero_page; /* Zero page addressing -> 0x00 => 0xff */
  uint16_t abs;      /* Access to all pages -> 0x0000 => 0xffff */
  uint8_t lo;
  uint8_t hi;

  switch (mode) {
  case (Immediate):
    /* Next byte, and uses it as an immediate value */
    out = fetch_byte(core);
    break;

  case (ZeroPage):
    /* Next byte is used as an address, 0-page => first half of memory */
    zero_page = fetch_byte(core);
    out = mem_read(core, zero_page);
    break;

  case (ZeroPage_X):;
    /* Next byte + X-reg is used as an address, 0-page wrap-around */
    zero_page = fetch_byte(core);
    out = mem_read(core, wrap_around_byte_add(zero_page, core->regs->X));
    break;

  case (ZeroPage_Y):;
    /* Next byte + Y-reg is used as an address, 0-page wrap-around */
    zero_page = fetch_byte(core);
    out = mem_read(core, wrap_around_byte_add(zero_page, core->regs->Y));
    break;

  case (Absolute):
    /* Next 2 bytes are used as an address */
    abs = fetch_two_bytes(core);
    out = mem_read(core, abs);
    break;

  case (Absolute_X):
    /* Next 2 bytes+ X-reg are used as an address, full memory wrap-around */
    abs = fetch_two_bytes(core);
    out = mem_read(core, wrap_around_2byte_add(abs, core->regs->X));
    break;

  case (Absolute_Y):
    /* Next 2 bytes+ Y-reg are used as an address, full memory wrap-around */
    abs = fetch_two_bytes(core);
    out = mem_read(core, wrap_around_2byte_add(abs, core->regs->Y));
    break;

  case (Indirect):
    /* Indirect pointer; address => two bytes == actual deref address => data */
    abs = fetch_two_bytes(core);
    lo = mem_read(core, abs);
    hi = mem_read(core, abs + 1);
    abs = (hi << 8) | lo;
    out = mem_read(core, abs);
    break;

  case (Indirect_X):
    /* Indirect pointer; address + X-Reg */
    zero_page = fetch_byte(core);
    abs = wrap_around_byte_add(zero_page, core->regs->X);
    lo = mem_read(core, abs);
    hi = mem_read(core, abs + 1);
    abs = (hi << 8) | lo;
    out = mem_read(core, abs);
    break;

  case (Indirect_Y):
    /* Indirect pointer; address + Y-Reg */
    zero_page = fetch_byte(core);
    abs = wrap_around_byte_add(zero_page, core->regs->Y);
    lo = mem_read(core, abs);
    hi = mem_read(core, abs + 1);
    abs = (hi << 8) | lo;
    out = mem_read(core, abs);
    break;

  case (NoneAddressing):
    break;
  default:
    printf("Invalid adressing mode %d! \n", mode);
    exit(-1);
  }
  return out;
}
