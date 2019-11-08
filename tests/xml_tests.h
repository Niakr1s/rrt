#ifndef XML_TESTS_H
#define XML_TESTS_H

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include <gtest/gtest.h>
#include "../src/common.h"
#include "../src/xml.h"
#include "../src/xmlcadastralnumber.h"

static const std::string PATH = "../../tests_data/";
static const std::string BLOCK = PATH + "KPT CadastralBlock 77 03 0009007.xml";

TEST(xml, constructor) {
  auto xml = rrt::XML(BLOCK.c_str());
  ASSERT_NE(xml.cadastralNumbers().size(), 0);
  ASSERT_EQ(xml.cadastralNumbers().size(), 25);
}

TEST(xml, intersects) {
  auto xml = rrt::XML(BLOCK.c_str());
  const auto& cns = xml.cadastralNumbers();

  linestring_t line1{
      {8854.7, 23408.53},
      {8897.7, 23434.53},
      {8867.7, 23485.53},
  };

  linestring_t line2{
      {8654.7, 23408.53},
      {8697.7, 23434.53},
      {8667.7, 23485.53},
  };

  ASSERT_TRUE(cns[0].intersects(line1));
  ASSERT_FALSE(cns[0].intersects(line2));

  std::vector<rrt::XMLCadastralNumber> res;
  std::copy_if(
      cns.begin(), cns.end(), std::back_inserter(res),
      [&](const rrt::XMLCadastralNumber& it) { return it.intersects(line1); });
  ASSERT_EQ(res.size(), 2);
}

#endif  // XML_TESTS_H
