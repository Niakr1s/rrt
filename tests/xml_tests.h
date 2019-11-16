#ifndef XML_TESTS_H
#define XML_TESTS_H

#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <gtest/gtest.h>
#include "xml.h"
#include "xmlspatial.h"

extern std::string PATH;
extern const std::string BLOCK;

TEST(xml, constructor) {
  auto xml = rrt::XML(BLOCK.c_str());
  ASSERT_NE(xml.xmlSpatials().size(), 0);
  ASSERT_EQ(xml.xmlSpatials().size(), 25);
}

TEST(xml, xmlInfo) {
  auto xml = rrt::XML(BLOCK.c_str());
  ASSERT_EQ(xml.xmlInfo().spatialInfo().cadastralNumber().string(),
            "77:03:0009007");
  ASSERT_EQ(xml.xmlInfo().spatialInfo().cadastralNumber().underscoredString(),
            "77_03_0009007");
  ASSERT_EQ(xml.xmlInfo().spatialInfo().type(), "CadastralBlock");
  ASSERT_EQ(xml.xmlInfo().date(), "2018-10-23");
  ASSERT_EQ(xml.xmlInfo().type(), "KPT");
  ASSERT_EQ(xml.xmlInfo().orderNumber(), "99/2018/207988061");
}

TEST(xml, intersects) {
  auto xml = rrt::XML(BLOCK.c_str());
  auto& cns = xml.xmlSpatials();

  rrt::Spatial::linestring_t line1{
      {8854.7, 23408.53},
      {8897.7, 23434.53},
      {8867.7, 23485.53},
  };

  rrt::Spatial::linestring_t line2{
      {8654.7, 23408.53},
      {8697.7, 23434.53},
      {8667.7, 23485.53},
  };

  rrt::Spatial::linestring_t line3{
      {8891.8963, 23480.9723},
      {8894.9634, 23480.9723},
      {8890.3796, 23483.3805},
  };

  ASSERT_TRUE(cns[0]->spatial()->intersects(line1));
  ASSERT_FALSE(cns[0]->spatial()->intersects(line2));

  std::vector<std::shared_ptr<rrt::XMLSpatial>> res;
  std::copy_if(cns.begin(), cns.end(), std::back_inserter(res),
               [&](std::shared_ptr<rrt::XMLSpatial>& it) {
                 return it->spatial()->intersects(line1);
               });
  ASSERT_EQ(res.size(), 2);

  res.clear();
  std::copy_if(cns.begin(), cns.end(), std::back_inserter(res),
               [&](std::shared_ptr<rrt::XMLSpatial>& it) {
                 return it->spatial()->intersects(line3);
               });
  ASSERT_EQ(res.size(), 1);
  ASSERT_EQ(res[0]->info().cadastralNumber().string(), "77:03:0009007");
}

#endif  // XML_TESTS_H
