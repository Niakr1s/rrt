#ifndef BOOST_TESTS_H
#define BOOST_TESTS_H

#include <iostream>

#include <gtest/gtest.h>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include "../src/common.h"

/*
TEST(boost, polygonIntersection1) {
  polygon_t polygon{
      {{0.0, 0.0},
       {20.0, 50.0},
       {110.0, 80.0},
       {143.0, -19.0},
       {59.0, -47.0},
       {0.0, 0.0}},
      {{30.0, 10.0}, {40.0, 40.0}, {60.0, 10.0}, {50.0, -20.0}, {30.0, 10.0}},
      {{70.0, 20.0},
       {100.0, 60.0},
       {120.0, -10.0},
       {80.0, -21.0},
       {70.0, 20.0}}};

  bg::correct(polygon);

  polygon_t polygon1{{{90, 20}, {100, 10}, {90, 0}, {90, 20}}};
  polygon_t polygon2{{{80, 40}, {100, 10}, {90, 0}, {80, 40}}};
  EXPECT_TRUE(bg::is_valid(polygon));
  EXPECT_TRUE(bg::is_valid(polygon1));

  EXPECT_TRUE(bg::is_valid(polygon2));
  EXPECT_FALSE(bg::intersects(polygon, polygon1));
  EXPECT_TRUE(bg::intersects(polygon, polygon2));
}

TEST(boost, polygonIntersection2) {
  mlinestring_t polygon{
      {{0.0, 0.0}, {20.0, 50.0}, {110.0, 80.0}, {143.0, -19.0}, {59.0, -47.0}}};
  polygon_t polygon1{{{90, 20}, {100, 10}, {90, 0}, {90, 20}}};
  polygon_t polygon2{{{80, 40}, {100, 10}, {90, 0}, {80, 40}}};
  bg::correct(polygon);
  bg::correct(polygon1);
  bg::correct(polygon2);
  EXPECT_TRUE(bg::is_valid(polygon1));
  EXPECT_TRUE(bg::is_valid(polygon2));
  EXPECT_FALSE(bg::intersects(polygon, polygon1));
  EXPECT_FALSE(bg::intersects(polygon, polygon2));
}

TEST(boost, CircleIntersection2) {
  polygon_t polygon{
      {{0.0, 0.0},
       {20.0, 50.0},
       {110.0, 80.0},
       {143.0, -19.0},
       {59.0, -47.0},
       {0.0, 0.0}},
      {{30.0, 10.0}, {40.0, 40.0}, {60.0, 10.0}, {50.0, -20.0}, {30.0, 10.0}},
      {{70.0, 20.0},
       {100.0, 60.0},
       {120.0, -10.0},
       {80.0, -21.0},
       {70.0, 20.0}}};
  bg::correct(polygon);

  bg::strategy::buffer::point_circle point_strategy(360);
  bg::strategy::buffer::distance_symmetric<double> distance_strategy10(10.0);
  bg::strategy::buffer::distance_symmetric<double> distance_strategy20(20.0);
  bg::strategy::buffer::join_round join_strategy;
  bg::strategy::buffer::end_round end_strategy;
  bg::strategy::buffer::side_straight side_strategy;

  point_t p{90.0, 10.0};
  mpolygon_t p10;
  mpolygon_t p20;

  bg::buffer(p, p10, distance_strategy10, side_strategy, join_strategy,
             end_strategy, point_strategy);
  bg::buffer(p, p20, distance_strategy20, side_strategy, join_strategy,
             end_strategy, point_strategy);

  EXPECT_FALSE(bg::intersects(polygon, p10));
  EXPECT_TRUE(bg::intersects(polygon, p20));
} */

#endif  // BOOST_TESTS_H
