#include <cstdio>
#include <gtest/gtest.h>
extern "C" {
#include "../../../include/instruction_set.h"
#include "../../../include/mem.h"
}

namespace {
TEST(testLDA, LDA) {
  cpu *core = init_cpu();
  *(core->storage->mem + 0x8000) = 0xa9;
  *(core->storage->mem + 0x8000 + 1) = 0x05;
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
  *(core->storage->mem + 0x8000) = 0xaa;
  *(core->storage->mem + 0x8000 + 1) = 0x00;
  run_cpu(core);

  EXPECT_EQ(core->regs->X, 10);
  EXPECT_EQ(core->regs->stat & 0x02, 0); // 0 flag
  EXPECT_EQ(core->regs->stat & 0x80, 0); // negative flag
}

TEST(test5s, test5) {

  cpu *core = init_cpu();
  
  *(core->storage->mem + 0x8000) = 0xa9;
  *(core->storage->mem + 0x8000 + 1) = 0xc0;
  *(core->storage->mem + 0x8000 + 2) = 0xaa;
  *(core->storage->mem + 0x8000 + 3) = 0xe8;
  *(core->storage->mem + 0x8000 + 4) = 0x00;
  run_cpu(core);

  EXPECT_EQ(core->regs->X, 0xc1);
}

TEST(testInxOverflow, inx) {
  cpu *core = init_cpu();
  
  core->regs->X = 0xff;
  *(core->storage->mem + 0x8000) = 0xe8;
  *(core->storage->mem + 0x8000 + 1) = 0xe8;
  *(core->storage->mem + 0x8000 + 2) = 0x00;
  run_cpu(core);

  EXPECT_EQ(core->regs->X, 1);
}
} // namespace
