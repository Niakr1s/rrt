#ifndef INTERSECTS_TESTS_H
#define INTERSECTS_TESTS_H

#include <gtest/gtest.h>
#include <string>

#include "dxf.h"
#include "xml.h"

extern std::string PATH;
extern const std::string BLOCK;

const std::string file = PATH + "intersects_false.dwg";
const std::string file1 = PATH + "intersects_true1.dwg";
const std::string file2 = PATH + "intersects_true2.dwg";
const std::string file3 = PATH + "intersects_true3.dwg";

const auto xml = rrt::XML(BLOCK.c_str());

TEST(intersects, test0) {
  rrt::DXF dxf;
  dxf.fileImport(file);

  auto res = xml.intersects(*dxf.spatial());
  ASSERT_EQ(res.size(), 1);
  ASSERT_EQ(res[0]->xmlSpatialInfo().cadastralNumber().string(),
            "77:03:0009007");
}

TEST(intersects, test1) {
  rrt::DXF dxf;
  dxf.fileImport(file1);

  auto res = xml.intersects(*dxf.spatial());
  ASSERT_EQ(res.size(), 2);
}

TEST(intersects, test2) {
  rrt::DXF dxf;
  dxf.fileImport(file2);

  auto res = xml.intersects(*dxf.spatial());
  ASSERT_EQ(res.size(), 2);
}

TEST(intersects, test3) {
  rrt::DXF dxf;
  dxf.fileImport(file3);

  auto res = xml.intersects(*dxf.spatial());
  ASSERT_EQ(res.size(), 2);
}

#endif  // INTERSECTS_TESTS_H
