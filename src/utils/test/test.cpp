#include <cstdio>
#include <gtest/gtest.h>
extern "C" {
#include "../../../include/utils.h"
}

namespace {
  TEST(testEndian, endian) {
    uint16_t out = switch_endian(0xc3af);
    EXPECT_EQ(out, 0xafc3);
    
    uint16_t switcher = switch_endian(out);
    EXPECT_EQ(switcher, 0xc3af);
  }

}

