#include "gtest/gtest.h"
#include "Algoritmos de compresion/WFC.h"

namespace {
  class WFCTest : public ::testing::Test {
  protected:
    WFC wfc;
  };
  TEST_F(WFCTest, actualizacionDeLista) {
    ASSERT_EQ(65,wfc.comprimir('A'));
    ASSERT_EQ(0,wfc.comprimir('A'));
  }
}
