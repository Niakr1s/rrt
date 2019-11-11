#ifndef DXF_TESTS_H
#define DXF_TESTS_H

#include <gtest/gtest.h>

#include "dxf.h"

static const std::string PATHD = "../../tests_data/";
static const std::string POLY = PATHD + "poly.dwg";

TEST(dxf, constructor) {
  rrt::DXF dxf(POLY);
}

#endif  // DXF_TESTS_H
