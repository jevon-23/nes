#include <cstdio>
#include <gtest/gtest.h>
extern "C" {
#include "../../../include/instruction_set.h"
#include "../../../include/mem.h"
}

namespace {
TEST(testSTA, STA_zero) {
  cpu *core = init_cpu();

  /* LDA 0x05 */
  *(core->storage->mem + 0x8000) = 0xa9; /* LDA ins top */
  *(core->storage->mem + 0x8000 + 1) = 0x05; /* num to be loaded in */

  /* STA into 0x20 */
  *(core->storage->mem + 0x8000 + 2) = 0x85;
  *(core->storage->mem + 0x8000 + 3) = 0x20;
  *(core->storage->mem + 0x8000 + 4) = 0x00;
  run_cpu(core);
  uint8_t a = mem_read(core, 0x0020);
  EXPECT_EQ(core->regs->A, 0x05); // make sure a register was set
  EXPECT_EQ(core->regs->stat & 0x02, 0); // 0 flag
  EXPECT_EQ(core->regs->stat & 0x80, 0); // negative flag
  EXPECT_EQ(a, 0x05); // make sure a register was set
  free_cpu(core);
}

TEST(testSTA, STA_zero_x) {
  cpu *core = init_cpu();

  /* Load a value into the x register */
  *(core->storage->mem + 0x8000) = 0xa2; /* LDX ins top */
  *(core->storage->mem + 0x8000 + 1) = 0x05; /* num to be loaded in */

  /* Store a value into the accumulator */
  *(core->storage->mem + 0x8000 + 2) = 0xa9; /* LDA ins top */
  *(core->storage->mem + 0x8000 + 3) = 0x55; /* num to be loaded in */

  /* STA into 0x20+X */
  *(core->storage->mem + 0x8000 + 4) = 0x95;
  *(core->storage->mem + 0x8000 + 5) = 0x20;
  *(core->storage->mem + 0x8000 + 6) = 0x00;

  run_cpu(core);
  uint8_t a = mem_read(core, 0x25);
  EXPECT_EQ(core->regs->X, 0x05); // make sure a register was set
  EXPECT_EQ(core->regs->stat & 0x02, 0); // 0 flag
  EXPECT_EQ(core->regs->stat & 0x80, 0); // negative flag
  EXPECT_EQ(core->regs->A, 0x55); // make sure a register was set
  EXPECT_EQ(a, 0x55); // make sure a register was set
  free_cpu(core);
}

TEST(testSTA, STA_zero_x_wrap) {
  cpu *core = init_cpu();

  /* Load a value into the x register */
  *(core->storage->mem + 0x8000) = 0xa2; /* LDX ins top */
  *(core->storage->mem + 0x8000 + 1) = 0x80; /* num to be loaded in */

  /* Store a value into the accumulator */
  *(core->storage->mem + 0x8000 + 2) = 0xa9; /* LDA ins top */
  *(core->storage->mem + 0x8000 + 3) = 0x55; /* num to be loaded in */

  /* STA into 0x20+X */
  *(core->storage->mem + 0x8000 + 4) = 0x95;
  *(core->storage->mem + 0x8000 + 5) = 0xff;
  *(core->storage->mem + 0x8000 + 6) = 0x00;

  run_cpu(core);
  uint8_t a = mem_read(core, 0x7f);
  EXPECT_EQ(core->regs->X, 0x80); // make sure a register was set
  EXPECT_EQ(core->regs->stat & 0x02, 0); // 0 flag
  EXPECT_EQ(core->regs->stat & 0x80, 0); // negative flag
  EXPECT_EQ(core->regs->A, 0x55); // make sure a register was set
  EXPECT_EQ(a, 0x55); // make sure a register was set
  free_cpu(core);
}

TEST(testSTA, STA_abs) {
  cpu *core = init_cpu();

  /* LDA 0x05 */
  *(core->storage->mem + 0x8000) = 0xa9; /* LDA ins top */
  *(core->storage->mem + 0x8000 + 1) = 0x05; /* num to be loaded in */

  /* STA into 0x20 */
  *(core->storage->mem + 0x8000 + 2) = 0x8d;
  *(core->storage->mem + 0x8000 + 3) = 0xef;
  *(core->storage->mem + 0x8000 + 4) = 0xbe;
  *(core->storage->mem + 0x8000 + 5) = 0x00;
  run_cpu(core);
  uint8_t a = mem_read(core, 0xbeef);
  EXPECT_EQ(core->regs->A, 0x05); // make sure a register was set
  EXPECT_EQ(core->regs->stat & 0x02, 0); // 0 flag
  EXPECT_EQ(core->regs->stat & 0x80, 0); // negative flag
  EXPECT_EQ(a, 0x05); // make sure a register was set
  free_cpu(core);
}

TEST(testSTA, STA_abs_x) {
  cpu *core = init_cpu();

  /* Load a value into the x register */
  *(core->storage->mem + 0x8000) = 0xa2; /* LDX ins top */
  *(core->storage->mem + 0x8000 + 1) = 0x0f; /* num to be loaded in */

  /* Store a value into the accumulator */
  *(core->storage->mem + 0x8000 + 2) = 0xa9; /* LDA ins top */
  *(core->storage->mem + 0x8000 + 3) = 0x55; /* num to be loaded in */

  /* STA into 0x20+X */
  *(core->storage->mem + 0x8000 + 4) = 0x9d;
  *(core->storage->mem + 0x8000 + 5) = 0xe0;
  *(core->storage->mem + 0x8000 + 6) = 0xbe;

  run_cpu(core);
  uint8_t a = mem_read(core, 0xbeef);
  EXPECT_EQ(core->regs->X, 0x0f); // make sure a register was set
  EXPECT_EQ(core->regs->stat & 0x02, 0); // 0 flag
  EXPECT_EQ(core->regs->stat & 0x80, 0); // negative flag
  EXPECT_EQ(core->regs->A, 0x55); // make sure a register was set
  EXPECT_EQ(a, 0x55); // make sure a register was set
  free_cpu(core);
}

TEST(testSTA, STA_abs_x_wrap) {
  cpu *core = init_cpu();

  /* Load a value into the x register */
  *(core->storage->mem + 0x8000) = 0xa2; /* LDX ins top */
  *(core->storage->mem + 0x8000 + 1) = 0x80; /* num to be loaded in */

  /* Store a value into the accumulator */
  *(core->storage->mem + 0x8000 + 2) = 0xa9; /* LDA ins top */
  *(core->storage->mem + 0x8000 + 3) = 0x55; /* num to be loaded in */

  /* STA into 0x20+X */
  *(core->storage->mem + 0x8000 + 4) = 0x9d;
  *(core->storage->mem + 0x8000 + 5) = 0xff;
  *(core->storage->mem + 0x8000 + 6) = 0xff;

  run_cpu(core);
  uint8_t a = mem_read(core, 0x7f);
  EXPECT_EQ(core->regs->X, 0x80); // make sure a register was set
  EXPECT_EQ(core->regs->stat & 0x02, 0); // 0 flag
  EXPECT_EQ(core->regs->stat & 0x80, 0); // negative flag
  EXPECT_EQ(core->regs->A, 0x55); // make sure a register was set
  EXPECT_EQ(a, 0x55); // make sure a register was set
  free_cpu(core);
}

TEST(testSTA, STA_abs_y) {
  cpu *core = init_cpu();

  /* Load a value into the y register */
  *(core->storage->mem + 0x8000) = 0xa0; /* LDX ins top */
  *(core->storage->mem + 0x8000 + 1) = 0x0f; /* num to be loaded in */

  /* Store a value into the accumulator */
  *(core->storage->mem + 0x8000 + 2) = 0xa9; /* LDA ins top */
  *(core->storage->mem + 0x8000 + 3) = 0x55; /* num to be loaded in */

  /* STA into 0x20+X */
  *(core->storage->mem + 0x8000 + 4) = 0x9d;
  *(core->storage->mem + 0x8000 + 5) = 0xe0;
  *(core->storage->mem + 0x8000 + 6) = 0xbe;

  run_cpu(core);
  uint8_t a = mem_read(core, 0xbeef);
  EXPECT_EQ(core->regs->Y, 0x0f); // make sure a register was set
  EXPECT_EQ(core->regs->stat & 0x02, 0); // 0 flag
  EXPECT_EQ(core->regs->stat & 0x80, 0); // negative flag
  EXPECT_EQ(core->regs->A, 0x55); // make sure a register was set
  EXPECT_EQ(a, 0x55); // make sure a register was set
  free_cpu(core);
}

TEST(testSTA, STA_ind_x) {
  cpu *core = init_cpu();

  mem_write(core, 0x1a, 0xef);
  mem_write(core, 0x1b, 0xbe);
  /* Load a value into the x register */
  *(core->storage->mem + 0x8000) = 0xa2; /* LDX ins top */
  *(core->storage->mem + 0x8000 + 1) = 0x05; /* num to be loaded in */

  /* Store a value into the accumulator */
  *(core->storage->mem + 0x8000 + 2) = 0xa9; /* LDA ins top */
  *(core->storage->mem + 0x8000 + 3) = 0x55; /* num to be loaded in */

  /* STA indirect + x */
  *(core->storage->mem + 0x8000 + 4) = 0x81; 
  *(core->storage->mem + 0x8000 + 5) = 0x15; /* num to be loaded in */
  *(core->storage->mem + 0x8000 + 6) = 0x00;
  
  run_cpu(core);
  uint8_t a = mem_read(core, 0xbeef);
  EXPECT_EQ(core->regs->X, 0x05); // make sure a register was set
  EXPECT_EQ(core->regs->stat & 0x02, 0); // 0 flag
  EXPECT_EQ(core->regs->stat & 0x80, 0); // negative flag
  EXPECT_EQ(core->regs->A, 0x55); // make sure a register was set
  EXPECT_EQ(a, 0x55); // make sure a register was set
  free_cpu(core);
}

TEST(testSTA, STA_ind_y) {
  cpu *core = init_cpu();

  mem_write(core, 0x1a, 0xef);
  mem_write(core, 0x1b, 0xbe);
  /* Load a value into the x register */
  *(core->storage->mem + 0x8000) = 0xa0; /* LDX ins top */
  *(core->storage->mem + 0x8000 + 1) = 0x05; /* num to be loaded in */

  /* Store a value into the accumulator */
  *(core->storage->mem + 0x8000 + 2) = 0xa9; /* LDA ins top */
  *(core->storage->mem + 0x8000 + 3) = 0x55; /* num to be loaded in */

  /* STA indirect + y */
  *(core->storage->mem + 0x8000 + 4) = 0x91; 
  *(core->storage->mem + 0x8000 + 5) = 0x15; /* num to be loaded in */
  *(core->storage->mem + 0x8000 + 6) = 0x00;
  
  run_cpu(core);
  uint8_t a = mem_read(core, 0xbeef);
  EXPECT_EQ(core->regs->Y, 0x05); // make sure a register was set
  EXPECT_EQ(core->regs->stat & 0x02, 0); // 0 flag
  EXPECT_EQ(core->regs->stat & 0x80, 0); // negative flag
  EXPECT_EQ(core->regs->A, 0x55); // make sure a register was set
  EXPECT_EQ(a, 0x55); // make sure a register was set
  free_cpu(core);
}


TEST(testLDY, LDY_Imm) {
  cpu *core = init_cpu();
  *(core->storage->mem + 0x8000) = 0xa0; /* LDY ins top */
  *(core->storage->mem + 0x8000 + 1) = 0x05; /* num to be loaded in */
  *(core->storage->mem + 0x8000 + 2) = 0x00;
  run_cpu(core);
  EXPECT_EQ(core->regs->Y, 0x05); // make sure a register was set
  EXPECT_EQ(core->regs->stat & 0x02, 0); // 0 flag
  EXPECT_EQ(core->regs->stat & 0x80, 0); // negative flag
  free_cpu(core);
}

TEST(testLDY, LDY_zero) {
  cpu *core = init_cpu();
  mem_write(core, 0x10, 0x55);
  *(core->storage->mem + 0x8000) = 0xa4; /* LDA ins top */
  *(core->storage->mem + 0x8000 + 1) = 0x10; /* num to be loaded in */
  *(core->storage->mem + 0x8000 + 2) = 0x00;
  run_cpu(core);
  EXPECT_EQ(core->regs->Y, 0x55); // make sure a register was set
  free_cpu(core);
}

TEST(testLDY, LDY_zero_x) {
  cpu *core = init_cpu();
  mem_write(core, 0x15, 0x55);
  /* Load a value into the x register */
  *(core->storage->mem + 0x8000) = 0xa2; /* LDX ins top */
  *(core->storage->mem + 0x8000 + 1) = 0x05; /* num to be loaded in */

  /* LDA zero page + X */
  *(core->storage->mem + 0x8000 + 2) = 0xb4; /* LDA ins top */
  *(core->storage->mem + 0x8000 + 3) = 0x10; /* num to be loaded in */
  *(core->storage->mem + 0x8000 + 4) = 0x00;
  run_cpu(core);
  EXPECT_EQ(core->regs->X, 0x05); // make sure a register was set
  EXPECT_EQ(core->regs->stat & 0x02, 0); // 0 flag
  EXPECT_EQ(core->regs->stat & 0x80, 0); // negative flag
  EXPECT_EQ(core->regs->Y, 0x55); // make sure a register was set
  free_cpu(core);
}

TEST(testLDY, LDY_zero_x_wrap) {
  cpu *core = init_cpu();
  mem_write(core, 0x7f, 0x55);
  /* Load a value into the x register */
  *(core->storage->mem + 0x8000) = 0xa2; /* LDX ins top */
  *(core->storage->mem + 0x8000 + 1) = 0x80; /* num to be loaded in */

  /* LDA zero page + X */
  *(core->storage->mem + 0x8000 + 2) = 0xb4; /* LDA ins top */
  *(core->storage->mem + 0x8000 + 3) = 0xff; /* num to be loaded in */
  *(core->storage->mem + 0x8000 + 4) = 0x00;
  run_cpu(core);
  EXPECT_EQ(core->regs->X, 0x80); // make sure a register was set
  EXPECT_EQ(core->regs->stat & 0x02, 0); // 0 flag
  EXPECT_EQ(core->regs->stat & 0x80, 0); // negative flag
  EXPECT_EQ(core->regs->Y, 0x55); // make sure a register was set
  free_cpu(core);
}

TEST(testLDY, LDY_abs) {
  cpu *core = init_cpu();
  mem_write(core, 0xbeef, 0x55);
  *(core->storage->mem + 0x8000) = 0xac; /* LDA ins top */
  *(core->storage->mem + 0x8000 + 1) = 0xef; /* num to be loaded in */
  *(core->storage->mem + 0x8000 + 2) = 0xbe;
  *(core->storage->mem + 0x8000 + 3) = 0x00;
  run_cpu(core);
  EXPECT_EQ(core->regs->Y, 0x55); // make sure a register was set
  free_cpu(core);
}

TEST(testLDY, LDY_abs_x) {
  cpu *core = init_cpu();
  mem_write(core, 0xbeef, 0x55);

  /* Load a value into the x register */
  *(core->storage->mem + 0x8000) = 0xa2; /* LDX ins top */
  *(core->storage->mem + 0x8000 + 1) = 0x0f; /* num to be loaded in */

  /* LDA abs x */
  *(core->storage->mem + 0x8000 + 2) = 0xbc; /* LDA ins top */
  *(core->storage->mem + 0x8000 + 3) = 0xe0; /* num to be loaded in */
  *(core->storage->mem + 0x8000 + 4) = 0xbe;
  *(core->storage->mem + 0x8000 + 5) = 0x00;

  run_cpu(core);
  EXPECT_EQ(core->regs->X, 0x0f); // make sure a register was set
  EXPECT_EQ(core->regs->stat & 0x02, 0); // 0 flag
  EXPECT_EQ(core->regs->stat & 0x80, 0); // negative flag
  EXPECT_EQ(core->regs->Y, 0x55); // make sure a register was set
  free_cpu(core);
}

TEST(testLDY, LDY_abs_x_wrap) {
  cpu *core = init_cpu();
  mem_write(core, 0x7f, 0x55);

  /* Load a value into the x register */
  *(core->storage->mem + 0x8000) = 0xa2; /* LDX ins top */
  *(core->storage->mem + 0x8000 + 1) = 0x80; /* num to be loaded in */

  /* LDA abs x */
  *(core->storage->mem + 0x8000 + 2) = 0xbc; /* LDA ins top */
  *(core->storage->mem + 0x8000 + 3) = 0xff; /* num to be loaded in */
  *(core->storage->mem + 0x8000 + 4) = 0xff;
  *(core->storage->mem + 0x8000 + 5) = 0x00;

  run_cpu(core);
  EXPECT_EQ(core->regs->X, 0x80); // make sure a register was set
  EXPECT_EQ(core->regs->stat & 0x02, 0); // 0 flag
  EXPECT_EQ(core->regs->stat & 0x80, 0); // negative flag
  EXPECT_EQ(core->regs->Y, 0x55); // make sure a register was set
  free_cpu(core);
}

TEST(testLDX, LDX_Imm) {
  cpu *core = init_cpu();
  *(core->storage->mem + 0x8000) = 0xa2; /* LDX ins top */
  *(core->storage->mem + 0x8000 + 1) = 0x05; /* num to be loaded in */
  *(core->storage->mem + 0x8000 + 2) = 0x00;
  run_cpu(core);
  EXPECT_EQ(core->regs->X, 0x05); // make sure a register was set
  EXPECT_EQ(core->regs->stat & 0x02, 0); // 0 flag
  EXPECT_EQ(core->regs->stat & 0x80, 0); // negative flag
  free_cpu(core);
}

TEST(testLDX, LDX_zero) {
  cpu *core = init_cpu();
  mem_write(core, 0x10, 0x55);
  *(core->storage->mem + 0x8000) = 0xa6; /* LDA ins top */
  *(core->storage->mem + 0x8000 + 1) = 0x10; /* num to be loaded in */
  *(core->storage->mem + 0x8000 + 2) = 0x00;
  run_cpu(core);
  EXPECT_EQ(core->regs->X, 0x55); // make sure a register was set
  free_cpu(core);
}

TEST(testLDX, LDX_zero_y) {
  cpu *core = init_cpu();
  mem_write(core, 0x15, 0x55);
  /* Load a value into the x register */
  *(core->storage->mem + 0x8000) = 0xa0; /* LDX ins top */
  *(core->storage->mem + 0x8000 + 1) = 0x05; /* num to be loaded in */

  /* LDX zero page + Y */
  *(core->storage->mem + 0x8000 + 2) = 0xb6; /* LDA ins top */
  *(core->storage->mem + 0x8000 + 3) = 0x10; /* num to be loaded in */
  *(core->storage->mem + 0x8000 + 4) = 0x00;
  run_cpu(core);
  EXPECT_EQ(core->regs->Y, 0x05); // make sure a register was set
  EXPECT_EQ(core->regs->stat & 0x02, 0); // 0 flag
  EXPECT_EQ(core->regs->stat & 0x80, 0); // negative flag
  EXPECT_EQ(core->regs->X, 0x55); // make sure a register was set
  free_cpu(core);
}

TEST(testLDX, LDX_zero_y_wrap) {
  cpu *core = init_cpu();
  mem_write(core, 0x7f, 0x55);
  /* Load a value into the x register */
  *(core->storage->mem + 0x8000) = 0xa0; /* LDX ins top */
  *(core->storage->mem + 0x8000 + 1) = 0x80; /* num to be loaded in */

  /* LDX zero page + Y */
  *(core->storage->mem + 0x8000 + 2) = 0xb6; /* LDA ins top */
  *(core->storage->mem + 0x8000 + 3) = 0xff; /* num to be loaded in */
  *(core->storage->mem + 0x8000 + 4) = 0x00;
  run_cpu(core);
  EXPECT_EQ(core->regs->Y, 0x80); // make sure a register was set
  EXPECT_EQ(core->regs->stat & 0x02, 0); // 0 flag
  EXPECT_EQ(core->regs->stat & 0x80, 0); // negative flag
  EXPECT_EQ(core->regs->X, 0x55); // make sure a register was set
  free_cpu(core);
}

TEST(testLDX, LDX_abs) {
  cpu *core = init_cpu();
  mem_write(core, 0xbeef, 0x55);
  *(core->storage->mem + 0x8000) = 0xae; /* LDA ins top */
  *(core->storage->mem + 0x8000 + 1) = 0xef; /* num to be loaded in */
  *(core->storage->mem + 0x8000 + 2) = 0xbe;
  *(core->storage->mem + 0x8000 + 3) = 0x00;
  run_cpu(core);
  EXPECT_EQ(core->regs->X, 0x55); // make sure a register was set
  free_cpu(core);
}

TEST(testLDX, LDX_abs_y) {
  cpu *core = init_cpu();
  mem_write(core, 0xbeef, 0x55);

  /* Load a value into the x register */
  *(core->storage->mem + 0x8000) = 0xa0; /* LDX ins top */
  *(core->storage->mem + 0x8000 + 1) = 0x0f; /* num to be loaded in */

  /* LDA abs x */
  *(core->storage->mem + 0x8000 + 2) = 0xbe; /* LDA ins top */
  *(core->storage->mem + 0x8000 + 3) = 0xe0; /* num to be loaded in */
  *(core->storage->mem + 0x8000 + 4) = 0xbe;
  *(core->storage->mem + 0x8000 + 5) = 0x00;

  run_cpu(core);
  EXPECT_EQ(core->regs->Y, 0x0f); // make sure a register was set
  EXPECT_EQ(core->regs->stat & 0x02, 0); // 0 flag
  EXPECT_EQ(core->regs->stat & 0x80, 0); // negative flag
  EXPECT_EQ(core->regs->X, 0x55); // make sure a register was set
  free_cpu(core);
}

TEST(testLDX, LDX_abs_y_wrap) {
  cpu *core = init_cpu();
  mem_write(core, 0xbeef, 0x55);

  /* Load a value into the x register */
  *(core->storage->mem + 0x8000) = 0xa0; /* LDX ins top */
  *(core->storage->mem + 0x8000 + 1) = 0x80; /* num to be loaded in */

  /* LDA abs x */
  *(core->storage->mem + 0x8000 + 2) = 0xbe; /* LDA ins top */
  *(core->storage->mem + 0x8000 + 3) = 0xff; /* num to be loaded in */
  *(core->storage->mem + 0x8000 + 4) = 0xff;
  *(core->storage->mem + 0x8000 + 5) = 0x00;

  run_cpu(core);
  EXPECT_EQ(core->regs->Y, 0x80); // make sure a register was set
  EXPECT_EQ(core->regs->stat & 0x02, 0); // 0 flag
  EXPECT_EQ(core->regs->stat & 0x80, 0); // negative flag
  EXPECT_EQ(core->regs->X, 0x55); // make sure a register was set
  free_cpu(core);
}

TEST(testLDA, LDA) {
  cpu *core = init_cpu();
  *(core->storage->mem + 0x8000) = 0xa9; /* LDA ins top */
  *(core->storage->mem + 0x8000 + 1) = 0x05; /* num to be loaded in */
  *(core->storage->mem + 0x8000 + 2) = 0x00;
  run_cpu(core);
  EXPECT_EQ(core->regs->A, 0x05); // make sure a register was set
  EXPECT_EQ(core->regs->stat & 0x02, 0); // 0 flag
  EXPECT_EQ(core->regs->stat & 0x80, 0); // negative flag
  free_cpu(core);
}

TEST(testLDA, LDA_zero) {
  cpu *core = init_cpu();
  mem_write(core, 0x10, 0x55);
  *(core->storage->mem + 0x8000) = 0xa5; /* LDA ins top */
  *(core->storage->mem + 0x8000 + 1) = 0x10; /* num to be loaded in */
  *(core->storage->mem + 0x8000 + 2) = 0x00;
  run_cpu(core);
  EXPECT_EQ(core->regs->A, 0x55); // make sure a register was set
  free_cpu(core);
}

TEST(testLDA, LDA_zero_x) {
  cpu *core = init_cpu();
  mem_write(core, 0x15, 0x55);
  /* Load a value into the x register */
  *(core->storage->mem + 0x8000) = 0xa2; /* LDX ins top */
  *(core->storage->mem + 0x8000 + 1) = 0x05; /* num to be loaded in */

  /* LDA zero page + X */
  *(core->storage->mem + 0x8000 + 2) = 0xb5; /* LDA ins top */
  *(core->storage->mem + 0x8000 + 3) = 0x10; /* num to be loaded in */
  *(core->storage->mem + 0x8000 + 4) = 0x00;
  run_cpu(core);
  EXPECT_EQ(core->regs->X, 0x05); // make sure a register was set
  EXPECT_EQ(core->regs->stat & 0x02, 0); // 0 flag
  EXPECT_EQ(core->regs->stat & 0x80, 0); // negative flag
  EXPECT_EQ(core->regs->A, 0x55); // make sure a register was set
  free_cpu(core);

}

TEST(testLDA, LDA_zero_x_wrap) {
  cpu *core = init_cpu();
  mem_write(core, 0x7F, 0x55);
  /* Load a value into the x register */
  *(core->storage->mem + 0x8000) = 0xa2; /* LDX ins top */
  *(core->storage->mem + 0x8000 + 1) = 0x80; /* num to be loaded in */

  /* LDA zero page + X on boundary */
  *(core->storage->mem + 0x8000 + 2) = 0xb5; /* LDA ins top */
  *(core->storage->mem + 0x8000 + 3) = 0xff; /* num to be loaded in */
  *(core->storage->mem + 0x8000 + 4) = 0x00;
  run_cpu(core);
  EXPECT_EQ(core->regs->X, 0x80); // make sure a register was set
  EXPECT_EQ(core->regs->stat & 0x02, 0); // 0 flag
  EXPECT_EQ(core->regs->stat & 0x80, 0); // negative flag
  EXPECT_EQ(core->regs->A, 0x55); // make sure a register was set
  free_cpu(core);
}

TEST(testLDA, LDA_abs) {
  cpu *core = init_cpu();
  mem_write(core, 0xbeef, 0x55);
  *(core->storage->mem + 0x8000) = 0xad; /* LDA ins top */
  *(core->storage->mem + 0x8000 + 1) = 0xef; /* num to be loaded in */
  *(core->storage->mem + 0x8000 + 2) = 0xbe;
  *(core->storage->mem + 0x8000 + 3) = 0x00;
  run_cpu(core);
  EXPECT_EQ(core->regs->A, 0x55); // make sure a register was set
  free_cpu(core);
}

TEST(testLDA, LDA_abs_x) {
  cpu *core = init_cpu();
  mem_write(core, 0xbeef, 0x55);

  /* Load a value into the x register */
  *(core->storage->mem + 0x8000) = 0xa2; /* LDX ins top */
  *(core->storage->mem + 0x8000 + 1) = 0x0f; /* num to be loaded in */

  /* LDA abs x */
  *(core->storage->mem + 0x8000 + 2) = 0xbd; /* LDA ins top */
  *(core->storage->mem + 0x8000 + 3) = 0xe0; /* num to be loaded in */
  *(core->storage->mem + 0x8000 + 4) = 0xbe;
  *(core->storage->mem + 0x8000 + 5) = 0x00;

  run_cpu(core);
  EXPECT_EQ(core->regs->X, 0x0f); // make sure a register was set
  EXPECT_EQ(core->regs->stat & 0x02, 0); // 0 flag
  EXPECT_EQ(core->regs->stat & 0x80, 0); // negative flag
  EXPECT_EQ(core->regs->A, 0x55); // make sure a register was set
  free_cpu(core);
}

TEST(testLDA, LDA_abs_x_wrap) {
  cpu *core = init_cpu();
  mem_write(core, 0x7f, 0x55);

  /* Load a value into the x register */
  *(core->storage->mem + 0x8000) = 0xa2; /* LDX ins top */
  *(core->storage->mem + 0x8000 + 1) = 0x80; /* num to be loaded in */

  /* LDA abs x */
  *(core->storage->mem + 0x8000 + 2) = 0xbd; /* LDA ins top */
  *(core->storage->mem + 0x8000 + 3) = 0xff; /* num to be loaded in */
  *(core->storage->mem + 0x8000 + 4) = 0xff;
  *(core->storage->mem + 0x8000 + 5) = 0x00;

  run_cpu(core);
  EXPECT_EQ(core->regs->X, 0x80); // make sure a register was set
  EXPECT_EQ(core->regs->stat & 0x02, 0); // 0 flag
  EXPECT_EQ(core->regs->stat & 0x80, 0); // negative flag
  EXPECT_EQ(core->regs->A, 0x55); // make sure a register was set
  free_cpu(core);
}

TEST(testLDA, LDA_abs_y) {
  cpu *core = init_cpu();
  mem_write(core, 0xbeef, 0x55);

  /* Load a value into the x register */
  *(core->storage->mem + 0x8000) = 0xa0; /* LDX ins top */
  *(core->storage->mem + 0x8000 + 1) = 0x0f; /* num to be loaded in */

  /* LDA abs x */
  *(core->storage->mem + 0x8000 + 2) = 0xb9; /* LDA ins top */
  *(core->storage->mem + 0x8000 + 3) = 0xe0; /* num to be loaded in */
  *(core->storage->mem + 0x8000 + 4) = 0xbe;
  *(core->storage->mem + 0x8000 + 5) = 0x00;

  run_cpu(core);
  EXPECT_EQ(core->regs->Y, 0x0f); // make sure a register was set
  EXPECT_EQ(core->regs->stat & 0x02, 0); // 0 flag
  EXPECT_EQ(core->regs->stat & 0x80, 0); // negative flag
  EXPECT_EQ(core->regs->A, 0x55); // make sure a register was set
  free_cpu(core);
}

TEST(testLDA, LDA_abs_y_wrap) {
  cpu *core = init_cpu();
  mem_write(core, 0x7f, 0x55);

  /* Load a value into the x register */
  *(core->storage->mem + 0x8000) = 0xa0; /* LDX ins top */
  *(core->storage->mem + 0x8000 + 1) = 0x80; /* num to be loaded in */

  /* LDA abs x */
  *(core->storage->mem + 0x8000 + 2) = 0xb9; /* LDA ins top */
  *(core->storage->mem + 0x8000 + 3) = 0xff; /* num to be loaded in */
  *(core->storage->mem + 0x8000 + 4) = 0xff;
  *(core->storage->mem + 0x8000 + 5) = 0x00;

  run_cpu(core);
  EXPECT_EQ(core->regs->Y, 0x80); // make sure a register was set
  EXPECT_EQ(core->regs->stat & 0x02, 0); // 0 flag
  EXPECT_EQ(core->regs->stat & 0x80, 0); // negative flag
  EXPECT_EQ(core->regs->A, 0x55); // make sure a register was set
  free_cpu(core);
}

TEST(testLDA, LDA_ind_x) {
  cpu *core = init_cpu();
  mem_write(core, 0xbeef, 0x55);
  mem_write(core, 0x1a, 0xef);
  mem_write(core, 0x1b, 0xbe);
  /* Load a value into the x register */
  *(core->storage->mem + 0x8000) = 0xa2; /* LDX ins top */
  *(core->storage->mem + 0x8000 + 1) = 0x05; /* num to be loaded in */

  /* LDA indirect  + X */
  *(core->storage->mem + 0x8000 + 2) = 0xA1; /* LDA ins top */
  *(core->storage->mem + 0x8000 + 3) = 0x15; /* num to be loaded in */
  *(core->storage->mem + 0x8000 + 4) = 0x00;
  run_cpu(core);
  EXPECT_EQ(core->regs->X, 0x05); // make sure a register was set
  EXPECT_EQ(core->regs->stat & 0x02, 0); // 0 flag
  EXPECT_EQ(core->regs->stat & 0x80, 0); // negative flag
  EXPECT_EQ(core->regs->A, 0x55); // make sure a register was set
  free_cpu(core);

}

TEST(testLDA, LDA_ind_y) {
  cpu *core = init_cpu();
  mem_write(core, 0xbeef, 0x55);
  mem_write(core, 0x1a, 0xef);
  mem_write(core, 0x1b, 0xbe);
  /* Load a value into the x register */
  *(core->storage->mem + 0x8000) = 0xa0; /* LDY ins top */
  *(core->storage->mem + 0x8000 + 1) = 0x05; /* num to be loaded in */

  /* LDA indirect  + X */
  *(core->storage->mem + 0x8000 + 2) = 0xB1; /* LDA ins top */
  *(core->storage->mem + 0x8000 + 3) = 0x15; /* num to be loaded in */
  *(core->storage->mem + 0x8000 + 4) = 0x00;
  run_cpu(core);
  EXPECT_EQ(core->regs->Y, 0x05); // make sure a register was set
  EXPECT_EQ(core->regs->stat & 0x02, 0); // 0 flag
  EXPECT_EQ(core->regs->stat & 0x80, 0); // negative flag
  EXPECT_EQ(core->regs->A, 0x55); // make sure a register was set
  free_cpu(core);

}

TEST(testTax, tax) {
  cpu *core = init_cpu();
  
  core->regs->A = 10;
  *(core->storage->mem + 0x8000) = 0xa9; /* LDA inst op */
  *(core->storage->mem + 0x8000 + 1) = 0x0a; /* Load 10 into A register */
  *(core->storage->mem + 0x8000 + 2) = 0xaa; /* Tax inst op */
  *(core->storage->mem + 0x8000 + 3) = 0x00;
  run_cpu(core);

  EXPECT_EQ(core->regs->A, 10);
  EXPECT_EQ(core->regs->stat & 0x02, 0); // 0 flag
  EXPECT_EQ(core->regs->stat & 0x80, 0); // negative flag
  EXPECT_EQ(core->regs->X, 10);
}

TEST(testTay, tay) {
  cpu *core = init_cpu();
  
  *(core->storage->mem + 0x8000) = 0xa9; /* LDA inst op */
  *(core->storage->mem + 0x8000 + 1) = 0x0a; /* Load 10 into A register */
  *(core->storage->mem + 0x8000 + 2) = 0xa8; /* Tay inst op */
  *(core->storage->mem + 0x8000 + 3) = 0x00;
  run_cpu(core);

  EXPECT_EQ(core->regs->A, 10);
  EXPECT_EQ(core->regs->stat & 0x02, 0); // 0 flag
  EXPECT_EQ(core->regs->stat & 0x80, 0); // negative flag
  EXPECT_EQ(core->regs->Y, 10);
}

TEST(testTxa, txa) {
  cpu *core = init_cpu();
  
  *(core->storage->mem + 0x8000) = 0xa2; /* LDX inst op */
  *(core->storage->mem + 0x8000 + 1) = 0x0a; /* Load 10 into A register */
  *(core->storage->mem + 0x8000 + 2) = 0x8a; /* Txa inst op */
  *(core->storage->mem + 0x8000 + 3) = 0x00;
  run_cpu(core);

  EXPECT_EQ(core->regs->X, 10);
  EXPECT_EQ(core->regs->stat & 0x02, 0); // 0 flag
  EXPECT_EQ(core->regs->stat & 0x80, 0); // negative flag
  EXPECT_EQ(core->regs->A, 10);
}

TEST(testTya, tya) {
  cpu *core = init_cpu();
  
  *(core->storage->mem + 0x8000) = 0xa0; /* LDY inst op */
  *(core->storage->mem + 0x8000 + 1) = 0x0a; /* Load 10 into A register */
  *(core->storage->mem + 0x8000 + 2) = 0x98; /* Txa inst op */
  *(core->storage->mem + 0x8000 + 3) = 0x00;
  run_cpu(core);

  EXPECT_EQ(core->regs->Y, 10);
  EXPECT_EQ(core->regs->stat & 0x02, 0); // 0 flag
  EXPECT_EQ(core->regs->stat & 0x80, 0); // negative flag
  EXPECT_EQ(core->regs->A, 10);
}

TEST(test5s, test5) {

  cpu *core = init_cpu();
  
  *(core->storage->mem + 0x8000) = 0xa9; /* LDA */
  *(core->storage->mem + 0x8000 + 1) = 0xc0; /* load 0xc0 into a reg */
  *(core->storage->mem + 0x8000 + 2) = 0xaa; /* Tax inst op */
  *(core->storage->mem + 0x8000 + 3) = 0xe8; /* Incx inst op */
  *(core->storage->mem + 0x8000 + 4) = 0x00;
  run_cpu(core);

  EXPECT_EQ(core->regs->X, 0xc1);
}

TEST(testInxOverflow, inx) {
  cpu *core = init_cpu();
  
  *(core->storage->mem + 0x8000) = 0xa9; /* LDA */
  *(core->storage->mem + 0x8000 + 1) = 0xff; /* Load ff into A reg */
  *(core->storage->mem + 0x8000 + 2) = 0xaa; /* Tax inst op */
  *(core->storage->mem + 0x8000 + 3) = 0xe8; /* inc x */
  *(core->storage->mem + 0x8000 + 4) = 0xe8; /* inc x again */
  *(core->storage->mem + 0x8000 + 5) = 0x00;
  run_cpu(core);

  EXPECT_EQ(core->regs->X, 1);
}

TEST(testWrapAdd, wrap_around_add) {
    uint8_t inp = 5;
    uint8_t off = 10;
    uint8_t out = wrap_around_byte_add(inp, off);
    EXPECT_EQ(out, 15);
    inp = 0x80;
    off = 0xff;
    out = wrap_around_byte_add(inp, off);
    EXPECT_EQ(out, 0x7f);


}

TEST(testWrap2Add, wrap_around2_add) {
    uint16_t inp = 5;
    uint16_t off = 10;
    uint16_t out = wrap_around_2byte_add(inp, off);
    EXPECT_EQ(out, 15);
    inp = 0x80;
    off = 0xffff;
    out = wrap_around_2byte_add(inp, off);
    EXPECT_EQ(out, 0x007f);

}

/* Tests out of order but this just how it is */

TEST(testSTX, STX_zero) {
  cpu *core = init_cpu();

  /* LDX 0x05 */
  *(core->storage->mem + 0x8000) = 0xa2; /* LDA ins top */
  *(core->storage->mem + 0x8000 + 1) = 0x05; /* num to be loaded in */

  /* STX into 0x20 */
  *(core->storage->mem + 0x8000 + 2) = 0x86;
  *(core->storage->mem + 0x8000 + 3) = 0x20;
  *(core->storage->mem + 0x8000 + 4) = 0x00;
  run_cpu(core);
  uint8_t x = mem_read(core, 0x0020);
  EXPECT_EQ(core->regs->X, 0x05); // make sure a register was set
  EXPECT_EQ(core->regs->stat & 0x02, 0); // 0 flag
  EXPECT_EQ(core->regs->stat & 0x80, 0); // negative flag
  EXPECT_EQ(x, 0x05); // make sure a register was set
  free_cpu(core);
}

TEST(testSTX, STX_zero_y) {
  cpu *core = init_cpu();

  /* Load a value into the y register */
  *(core->storage->mem + 0x8000) = 0xa0; /* LDX ins top */
  *(core->storage->mem + 0x8000 + 1) = 0x05; /* num to be loaded in */

  /* Store a value into the x register */
  *(core->storage->mem + 0x8000 + 2) = 0xa2; /* LDA ins top */
  *(core->storage->mem + 0x8000 + 3) = 0x55; /* num to be loaded in */

  /* STX into 0x20+X */
  *(core->storage->mem + 0x8000 + 4) = 0x96;
  *(core->storage->mem + 0x8000 + 5) = 0x20;
  *(core->storage->mem + 0x8000 + 6) = 0x00;

  run_cpu(core);
  uint8_t x = mem_read(core, 0x25);
  EXPECT_EQ(core->regs->Y, 0x05); // make sure a register was set
  EXPECT_EQ(core->regs->stat & 0x02, 0); // 0 flag
  EXPECT_EQ(core->regs->stat & 0x80, 0); // negative flag
  EXPECT_EQ(core->regs->X, 0x55); // make sure a register was set
  EXPECT_EQ(x, 0x55); // make sure a register was set
  free_cpu(core);
}

TEST(testSTX, STX_abs) {
  cpu *core = init_cpu();

  /* LDX 0x05 */
  *(core->storage->mem + 0x8000) = 0xa2; /* LDX ins top */
  *(core->storage->mem + 0x8000 + 1) = 0x05; /* num to be loaded in */

  /* STX into 0x20 */
  *(core->storage->mem + 0x8000 + 2) = 0x8e;
  *(core->storage->mem + 0x8000 + 3) = 0xef;
  *(core->storage->mem + 0x8000 + 4) = 0xbe;
  *(core->storage->mem + 0x8000 + 5) = 0x00;
  run_cpu(core);
  uint8_t x = mem_read(core, 0xbeef);
  EXPECT_EQ(core->regs->X, 0x05); // make sure a register was set
  EXPECT_EQ(core->regs->stat & 0x02, 0); // 0 flag
  EXPECT_EQ(core->regs->stat & 0x80, 0); // negative flag
  EXPECT_EQ(x, 0x05); // make sure a register was set
  free_cpu(core);
}

TEST(testSTY, STY_zero) {
  cpu *core = init_cpu();

  /* LDY 0x05 */
  *(core->storage->mem + 0x8000) = 0xa0; /* LDA ins top */
  *(core->storage->mem + 0x8000 + 1) = 0x05; /* num to be loaded in */

  /* STY into 0x20 */
  *(core->storage->mem + 0x8000 + 2) = 0x84;
  *(core->storage->mem + 0x8000 + 3) = 0x20;
  *(core->storage->mem + 0x8000 + 4) = 0x00;
  run_cpu(core);
  uint8_t y = mem_read(core, 0x0020);
  EXPECT_EQ(core->regs->Y, 0x05); // make sure a register was set
  EXPECT_EQ(core->regs->stat & 0x02, 0); // 0 flag
  EXPECT_EQ(core->regs->stat & 0x80, 0); // negative flag
  EXPECT_EQ(y, 0x05); // make sure a register was set
  free_cpu(core);
}

TEST(testSTY, STY_zero_x) {
  cpu *core = init_cpu();

  /* Load a value into the x register */
  *(core->storage->mem + 0x8000) = 0xa2; /* LDX ins top */
  *(core->storage->mem + 0x8000 + 1) = 0x05; /* num to be loaded in */

  /* Store a value into the y-register */
  *(core->storage->mem + 0x8000 + 2) = 0xa0; /* LDA ins top */
  *(core->storage->mem + 0x8000 + 3) = 0x55; /* num to be loaded in */

  /* STY into 0x20+X */
  *(core->storage->mem + 0x8000 + 4) = 0x96;
  *(core->storage->mem + 0x8000 + 5) = 0x20;
  *(core->storage->mem + 0x8000 + 6) = 0x00;

  run_cpu(core);
  uint8_t y = mem_read(core, 0x25);
  EXPECT_EQ(core->regs->X, 0x05); // make sure a register was set
  EXPECT_EQ(core->regs->stat & 0x02, 0); // 0 flag
  EXPECT_EQ(core->regs->stat & 0x80, 0); // negative flag
  EXPECT_EQ(core->regs->Y, 0x55); // make sure a register was set
  EXPECT_EQ(y, 0x55); // make sure a register was set
  free_cpu(core);
}

TEST(testSTY, STY_abs) {
  cpu *core = init_cpu();

  /* LDY 0x05 */
  *(core->storage->mem + 0x8000) = 0xa0; /* LDA ins top */
  *(core->storage->mem + 0x8000 + 1) = 0x05; /* num to be loaded in */

  /* STA into 0x20 */
  *(core->storage->mem + 0x8000 + 2) = 0x8c;
  *(core->storage->mem + 0x8000 + 3) = 0xef;
  *(core->storage->mem + 0x8000 + 4) = 0xbe;
  *(core->storage->mem + 0x8000 + 5) = 0x00;
  run_cpu(core);
  uint8_t y = mem_read(core, 0xbeef);
  EXPECT_EQ(core->regs->Y, 0x05); // make sure a register was set
  EXPECT_EQ(core->regs->stat & 0x02, 0); // 0 flag
  EXPECT_EQ(core->regs->stat & 0x80, 0); // negative flag
  EXPECT_EQ(y, 0x05); // make sure a register was set
  free_cpu(core);
}

TEST(testCPY, CPY_imm) {
  cpu *core = init_cpu();

  /* y < imm */
  /* LDY 0x05 */
  *(core->storage->mem + 0x8000) = 0xa0; /* LDY ins top */
  *(core->storage->mem + 0x8000 + 1) = 0x05; /* num to be loaded in */
  *(core->storage->mem + 0x8000 + 2) = 0xc0; /* cpy */
  *(core->storage->mem + 0x8000 + 3) = 0x10; /* num to be loaded in */
  *(core->storage->mem + 0x8000 + 4) = 0x00; /* num to be loaded in */
  run_cpu(core);
  EXPECT_EQ(core->regs->Y, 0x05); // make sure a register was set
  EXPECT_EQ(core->regs->stat & 0x01, 0x0); // 0 flag
  EXPECT_EQ(core->regs->stat & 0x2, 0x0); // 0 flag
  EXPECT_EQ(core->regs->stat & 0x08, 0x08); // negative flag
  free_cpu(core);

  cpu *core2 = init_cpu();

  /* y > imm */
  /* LDY 0x05 */
  *(core2->storage->mem + 0x8000) = 0xa0; /* LDY ins top */
  *(core2->storage->mem + 0x8000 + 1) = 0x05; /* num to be loaded in */
  *(core2->storage->mem + 0x8000 + 2) = 0xc0; /* cpy */
  *(core2->storage->mem + 0x8000 + 3) = 0x01; /* num to be loaded in */
  *(core2->storage->mem + 0x8000 + 4) = 0x00; /* num to be loaded in */
  run_cpu(core2);
  EXPECT_EQ(core2->regs->Y, 0x05); // make sure a register was set
  EXPECT_EQ(core2->regs->stat & 0x01, 0x1); // 0 flag
  EXPECT_EQ(core2->regs->stat & 0x2, 0x0); // 0 flag
  EXPECT_EQ(core2->regs->stat & 0x08, 0x00); // negative flag
  free_cpu(core2);

  cpu *core3 = init_cpu();

  /* y > imm */
  /* LDY 0x05 */
  *(core3->storage->mem + 0x8000) = 0xa0; /* LDY ins top */
  *(core3->storage->mem + 0x8000 + 1) = 0x05; /* num to be loaded in */
  *(core3->storage->mem + 0x8000 + 2) = 0xc0; /* cpy */
  *(core3->storage->mem + 0x8000 + 3) = 0x05; /* num to be loaded in */
  *(core3->storage->mem + 0x8000 + 4) = 0x00; /* num to be loaded in */
  run_cpu(core3);
  EXPECT_EQ(core3->regs->Y, 0x05); // make sure a register was set
  EXPECT_EQ(core3->regs->stat & 0x01, 0x1); // 0 flag
  EXPECT_EQ(core3->regs->stat & 0x2, 0x2); // 0 flag
  EXPECT_EQ(core3->regs->stat & 0x08, 0x00); // negative flag
  free_cpu(core3);
}

TEST(testCMP, CMP_imm) {
  cpu *core = init_cpu();

  /* a < imm */
  /* LDY 0x05 */
  *(core->storage->mem + 0x8000) = 0xa9; /* LDY ins top */
  *(core->storage->mem + 0x8000 + 1) = 0x05; /* num to be loaded in */
  *(core->storage->mem + 0x8000 + 2) = 0xc9; /* cpy */
  *(core->storage->mem + 0x8000 + 3) = 0x10; /* num to be loaded in */
  *(core->storage->mem + 0x8000 + 4) = 0x00; /* num to be loaded in */
  run_cpu(core);
  EXPECT_EQ(core->regs->A, 0x05); // make sure a register was set
  EXPECT_EQ(core->regs->stat & 0x01, 0x0); // 0 flag
  EXPECT_EQ(core->regs->stat & 0x2, 0x0); // 0 flag
  EXPECT_EQ(core->regs->stat & 0x08, 0x08); // negative flag
  free_cpu(core);

  cpu *core2 = init_cpu();

  /* y > imm */
  /* LDY 0x05 */
  *(core2->storage->mem + 0x8000) = 0xa9; /* LDY ins top */
  *(core2->storage->mem + 0x8000 + 1) = 0x05; /* num to be loaded in */
  *(core2->storage->mem + 0x8000 + 2) = 0xc9; /* cpy */
  *(core2->storage->mem + 0x8000 + 3) = 0x01; /* num to be loaded in */
  *(core2->storage->mem + 0x8000 + 4) = 0x00; /* num to be loaded in */
  run_cpu(core2);
  EXPECT_EQ(core2->regs->A, 0x05); // make sure a register was set
  EXPECT_EQ(core2->regs->stat & 0x01, 0x1); // 0 flag
  EXPECT_EQ(core2->regs->stat & 0x2, 0x0); // 0 flag
  EXPECT_EQ(core2->regs->stat & 0x08, 0x00); // negative flag
  free_cpu(core2);

  cpu *core3 = init_cpu();

  /* y > imm */
  /* LDY 0x05 */
  *(core3->storage->mem + 0x8000) = 0xa9; /* LDY ins top */
  *(core3->storage->mem + 0x8000 + 1) = 0x05; /* num to be loaded in */
  *(core3->storage->mem + 0x8000 + 2) = 0xc9; /* cpy */
  *(core3->storage->mem + 0x8000 + 3) = 0x05; /* num to be loaded in */
  *(core3->storage->mem + 0x8000 + 4) = 0x00; /* num to be loaded in */
  run_cpu(core3);
  EXPECT_EQ(core3->regs->A, 0x05); // make sure a register was set
  EXPECT_EQ(core3->regs->stat & 0x01, 0x1); // 0 flag
  EXPECT_EQ(core3->regs->stat & 0x2, 0x2); // 0 flag
  EXPECT_EQ(core3->regs->stat & 0x08, 0x00); // negative flag
  free_cpu(core3);
}

TEST(testCPX, CPX_imm) {
  cpu *core = init_cpu();

  /* x < imm */
  /* LDX 0x05 */
  *(core->storage->mem + 0x8000) = 0xa2; /* LDX ins top */
  *(core->storage->mem + 0x8000 + 1) = 0x05; /* num to be loaded in */
  *(core->storage->mem + 0x8000 + 2) = 0xe0; /* cpy */
  *(core->storage->mem + 0x8000 + 3) = 0x10; /* num to be loaded in */
  *(core->storage->mem + 0x8000 + 4) = 0x00; /* num to be loaded in */
  run_cpu(core);
  EXPECT_EQ(core->regs->X, 0x05); // make sure a register was set
  EXPECT_EQ(core->regs->stat & 0x01, 0x0); // 0 flag
  EXPECT_EQ(core->regs->stat & 0x2, 0x0); // 0 flag
  EXPECT_EQ(core->regs->stat & 0x08, 0x08); // negative flag
  free_cpu(core);

  cpu *core2 = init_cpu();

  /* x > imm */
  /* LDX 0x05 */
  *(core2->storage->mem + 0x8000) = 0xa2; /* LDX ins top */
  *(core2->storage->mem + 0x8000 + 1) = 0x05; /* num to be loaded in */
  *(core2->storage->mem + 0x8000 + 2) = 0xe0; /* cpy */
  *(core2->storage->mem + 0x8000 + 3) = 0x01; /* num to be loaded in */
  *(core2->storage->mem + 0x8000 + 4) = 0x00; /* num to be loaded in */
  run_cpu(core2);
  EXPECT_EQ(core2->regs->X, 0x05); // make sure a register was set
  EXPECT_EQ(core2->regs->stat & 0x01, 0x1); // 0 flag
  EXPECT_EQ(core2->regs->stat & 0x2, 0x0); // 0 flag
  EXPECT_EQ(core2->regs->stat & 0x08, 0x00); // negative flag
  free_cpu(core2);

  cpu *core3 = init_cpu();

  /* x > imm */
  /* LDX 0x05 */
  *(core3->storage->mem + 0x8000) = 0xa2; /* LDX ins top */
  *(core3->storage->mem + 0x8000 + 1) = 0x05; /* num to be loaded in */
  *(core3->storage->mem + 0x8000 + 2) = 0xe0; /* cpy */
  *(core3->storage->mem + 0x8000 + 3) = 0x05; /* num to be loaded in */
  *(core3->storage->mem + 0x8000 + 4) = 0x00; /* num to be loaded in */
  run_cpu(core3);
  EXPECT_EQ(core3->regs->X, 0x05); // make sure a register was set
  EXPECT_EQ(core3->regs->stat & 0x01, 0x1); // 0 flag
  EXPECT_EQ(core3->regs->stat & 0x2, 0x2); // 0 flag
  EXPECT_EQ(core3->regs->stat & 0x08, 0x00); // negative flag
  free_cpu(core3);
}

} // namespace
