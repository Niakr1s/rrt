#ifndef SQLLITE_TESTS_H
#define SQLLITE_TESTS_H

#include <gtest/gtest.h>

#include "db.h"
#include "sqldb.h"
#include "xml.h"

extern std::string PATH;
extern const std::string BLOCK;

TEST(sql, test1) {
  rrt::DB::set<rrt::SqlDB>();
  auto xml = rrt::XML(BLOCK.c_str());

  rrt::DB::get()->pushToDB(*xml.xmlSpatials()[0]);
}

#endif  // SQLLITE_TESTS_H
