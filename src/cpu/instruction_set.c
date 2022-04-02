#include "../../include/cpu.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void set_zero_flag(cpu *core, uint8_t val);
void set_negative_flag(cpu *core, uint8_t val);
void set_carry_flag(cpu *core, uint8_t val, bool set);
/* just trying to understand the flags, notice that if both negative flag
 * and zero flag are not set, we have 0xff, else we don't
 */

/* === instructions === */

/* Clear carry flag => set status reg = 0 */
int clc(cpu *core, uint8_t instruction) {
  core->regs->stat &= 0x00;
  return 0;
}

/* Set carry flag = 1 */
int sec(cpu *core, uint8_t instruction) {
  core->regs->stat &= 0x01; /* set flag = 1 */
  return 0;
}

/* Loads a byte of memroy into the accumulator */
int lda(cpu *core, uint8_t instruction) {

  /* set zero flag */
  set_zero_flag(core, instruction);

  /* set negative flag */
  set_negative_flag(core, instruction);

  /* set the A register */
  core->regs->A = instruction;

  return 0;
}

/* Load the value in the a register to accumulator */
int tax(cpu *core, uint8_t instruction) {
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
int inx(cpu *core, uint8_t instruction) {
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
int cpy(cpu *core, uint8_t instruction) {
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
  (set) ? sec(core, val) : clc(core, val);
}
