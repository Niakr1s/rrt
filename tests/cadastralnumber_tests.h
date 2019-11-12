#ifndef CADASTRALNUMBER_TESTS_H
#define CADASTRALNUMBER_TESTS_H

#include <gtest/gtest.h>
#include <string>

#include "cadastralnumber.h"

TEST(cadastralnumber, constructor) {
  rrt::CadastralNumber cn1("21:02");
  rrt::CadastralNumber cn2("21:01:000000");
  rrt::CadastralNumber cn12("21:01:01010101");
  rrt::CadastralNumber cn3("21:01:000000:221");
  rrt::CadastralNumber cn4("21:01:000000:223");

  ASSERT_TRUE(cn1 < cn2);
  ASSERT_TRUE(cn2 < cn3);
  ASSERT_TRUE(cn2 < cn12);
  ASSERT_TRUE(cn3 < cn4);
  ASSERT_NE(cn1, cn2);
}

#endif  // CADASTRALNUMBER_TESTS_H
