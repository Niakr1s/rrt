#include <gtest/gtest.h>

static std::string PATH = "../../tests_data/";

#include "boost_tests.h"
#include "dxf_tests.h"
#include "rect_tests.h"
#include "xml_tests.h"

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
