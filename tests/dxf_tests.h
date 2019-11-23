#ifndef DXF_TESTS_H
#define DXF_TESTS_H

#include <gtest/gtest.h>
#include <string>

#include "dxf.h"

extern std::string PATH;
static const std::string POLY = PATH + "poly.dxf";

TEST(dxf, constructor) {
  rrt::DXF dxf;
  dxf.fileImport(POLY);

  rrt::Spatial::linestring_t line1{
      {1076.0, 1347.0},
      {1000.0, 1534.0},
  };

  rrt::Spatial::linestring_t line2{
      {1350.0, 640.0},
      {1230.0, 900.0},
  };

  rrt::Spatial::linestring_t line3{
      {1495.0, 2253.0},
      {1728.0, 2587.0},
  };

  ASSERT_TRUE(dxf.spatial()->intersects(line1));
  ASSERT_TRUE(dxf.spatial()->intersects(line2));
  ASSERT_TRUE(dxf.spatial()->intersects(line3));

  rrt::Spatial::linestring_t line4{
      {2300.0, 1476.0},
      {2466.0, 1640.0},
  };

  rrt::Spatial::linestring_t line5{
      {2446.0, 1491.0},
      {2265.0, 1672.0},
  };

  ASSERT_FALSE(dxf.spatial()->intersects(line4));
  ASSERT_FALSE(dxf.spatial()->intersects(line5));
}

#endif  // DXF_TESTS_H
