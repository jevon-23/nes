#include <inttypes.h>
#include "cpu.h"
#ifndef INSTRUCTION_SET_H
#define INSTRUCTION_SET_H

/* 6502 instruction set */

/* load in the next byte from program into A register */
int lda (cpu *core, uint8_t instruction);

/* load the value inside of A regiester into X regiester */
int tax(cpu *core, uint8_t instruction);

/* increase X register by 1 */
int inx(cpu *core, uint8_t instruction);

/* compare the next byte from program w/ Y register */
int cpy(cpu *core, uint8_t instruction);

/* sets the status register to be 1 for the carry */
int sec(cpu *core, uint8_t instruction);

/* clears the status register for the carry flag */
int clc(cpu *core, uint8_t instruction);

#endif
