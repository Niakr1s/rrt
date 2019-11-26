#include <gtest/gtest.h>

static std::string PATH = "../../tests_data/";
static const std::string BLOCK = PATH + "KPT CadastralBlock 77 03 0009007.xml";

#include "boost_tests.h"
#include "cadastralnumber_tests.h"
#include "dxf_tests.h"
#include "intersects_tests.h"
#include "rect_tests.h"
#include "serialize_tests.h"
#include "xml_tests.h"

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
