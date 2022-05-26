#include "cpu.h"
#include <inttypes.h>
#include <stdlib.h>
#ifndef INSTRUCTION_SET_H
#define INSTRUCTION_SET_H

/* 6502 instruction set */

/* stores the value in the Y-register to a given address */
int sty(cpu *core, int mode);

/* stores the value in the X-register to a given address */
int stx(cpu *core, int mode);

/* stores the value in the accumulator to a given address */
int sta(cpu *core, int mode);

/* load in the next byte from program into A register */
int lda(cpu *core, int mode);

/* load in the next byte from program into X register */
int ldx(cpu *core, int mode);

/* load in the next byte from progarm into Y register */
int ldy(cpu *core, int mode);

/* load the value inside of A regiester into X regiester */
int tax(cpu *core);

/* load the value inside of A regiester into Y regiester */
int tay(cpu *core);

/* load the value inside of the stack pointer A regiester */
int tsx(cpu *core);

/* load the value inside of X regiester into A regiester */
int txa(cpu *core);

/* load the value inside of X regiester into stack pointer */
int txs(cpu *core);

/* load the value inside of Y regiester into A regiester */
int tya(cpu *core);

/* increase X register by 1 */
int inx(cpu *core);

/* compare the next byte from program w/ Y register */
int cpy(cpu *core, int mode);

/* compare the next byte from program w/ Y register */
int cpx(cpu *core, int mode);

/* sets the status register to be 1 for the carry */
int sec(cpu *core);

/* clears the status register for the carry flag */
int clc(cpu *core);

/* Adds INP + OFFSET in range of 1 byte [0-255]  */
size_t wrap_around_byte_add(size_t inp, size_t offset);

/* Adds INP + OFFSET in range of 2 bytes [0x0000-0xffff] */
size_t wrap_around_2byte_add(size_t inp, size_t offset);
#endif
