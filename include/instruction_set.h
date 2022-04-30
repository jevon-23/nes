#include "cpu.h"
#include <inttypes.h>
#include <stdlib.h>
#ifndef INSTRUCTION_SET_H
#define INSTRUCTION_SET_H

/* 6502 instruction set */

/* load in the next byte from program into A register */
int lda(cpu *core, int mode);

/* load the value inside of A regiester into X regiester */
int tax(cpu *core);

/* increase X register by 1 */
int inx(cpu *core);

/* compare the next byte from program w/ Y register */
int cpy(cpu *core, int mode);

/* sets the status register to be 1 for the carry */
int sec(cpu *core);

/* clears the status register for the carry flag */
int clc(cpu *core);

/* Adds INP + OFFSET in range of 1 byte [0-255]  */
size_t wrap_around_byte_add(size_t inp, size_t offset);

/* Adds INP + OFFSET in range of 2 bytes [0x0000-0xffff] */
size_t wrap_around_2byte_add(size_t inp, size_t offset);
#endif
