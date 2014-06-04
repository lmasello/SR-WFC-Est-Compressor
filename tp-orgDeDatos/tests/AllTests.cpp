#include "gtest.h"
#include "WFCTests.cpp"
#include "SymbolRankingTests.cpp"
#include "HashMapTests.cpp"

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


