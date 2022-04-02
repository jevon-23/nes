#include <cstdio>
#include <gtest/gtest.h>
extern "C" {
#include "../../../include/utils.h"
}

namespace {
TEST(testEndian, endian) {
  uint16_t out = big2lil_endian(0xc3af);
  EXPECT_EQ(out, 0xafc3);
}

}

