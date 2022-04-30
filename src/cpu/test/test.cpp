#include <cstdio>
#include <gtest/gtest.h>
extern "C" {
#include "../../../include/instruction_set.h"
#include "../../../include/mem.h"
}

namespace {
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

TEST(testTax, tax) {
  cpu *core = init_cpu();
  
  core->regs->A = 10;
  *(core->storage->mem + 0x8000) = 0xa9; /* LDA inst op */
  *(core->storage->mem + 0x8000 + 1) = 0x0a; /* Load 10 into A register */
  *(core->storage->mem + 0x8000 + 2) = 0xaa; /* Tax inst op */
  *(core->storage->mem + 0x8000 + 3) = 0x00;
  run_cpu(core);

  EXPECT_EQ(core->regs->X, 10);
  EXPECT_EQ(core->regs->stat & 0x02, 0); // 0 flag
  EXPECT_EQ(core->regs->stat & 0x80, 0); // negative flag
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
} // namespace
