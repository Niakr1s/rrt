#ifndef SQLLITE_TESTS_H
#define SQLLITE_TESTS_H

#include <gtest/gtest.h>

#include "cadastralnumber.h"
#include "db.h"
#include "sqldb.h"
#include "xml.h"

using namespace rrt;

extern std::string PATH;
extern const std::string BLOCK;
const std::string SQL_CAD_NO = "77:03:0009007:1225";

void init() {
  DB::set<rrt::SqlDB>();
  auto xml = XML(BLOCK.c_str());
  for (auto& s : xml.xmlSpatials()) {
    rrt::DB::get()->pushToDB(*s);
  }
}

TEST(sql, getFromDB1) {
  init();
  auto spa = DB::get()->getFromDB(CadastralNumber(SQL_CAD_NO));
  ASSERT_EQ(spa->xmlSpatialInfo().cadastralNumber().string(), SQL_CAD_NO);
  ASSERT_EQ(spa->xmlSpatialInfo().type(), "Parcel");

  spa = DB::get()->getFromDB(CadastralNumber(SQL_CAD_NO), "2018-10-23");
  ASSERT_EQ(spa->xmlSpatialInfo().cadastralNumber().string(), SQL_CAD_NO);
  ASSERT_EQ(spa->xmlSpatialInfo().type(), "Parcel");

  EXPECT_ANY_THROW(
      DB::get()->getFromDB(CadastralNumber(SQL_CAD_NO), "2000-10-23"));

  spa = DB::get()->getFromDB(CadastralNumber(SQL_CAD_NO), "",
                             "99/2018/207988061");
  ASSERT_EQ(spa->xmlSpatialInfo().cadastralNumber().string(), SQL_CAD_NO);
  ASSERT_EQ(spa->xmlSpatialInfo().type(), "Parcel");

  spa = DB::get()->getFromDB(CadastralNumber(SQL_CAD_NO), "2018-10-23",
                             "99/2018/207988061");
  ASSERT_EQ(spa->xmlSpatialInfo().cadastralNumber().string(), SQL_CAD_NO);
  ASSERT_EQ(spa->xmlSpatialInfo().type(), "Parcel");

  EXPECT_ANY_THROW(
      DB::get()->getFromDB(CadastralNumber(SQL_CAD_NO), "2018-10-23", "2355"));
}

#endif  // SQLLITE_TESTS_H
