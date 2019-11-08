#include <gtest/gtest.h>

#include "boost_tests.h"
#include "xml_tests.h"

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
