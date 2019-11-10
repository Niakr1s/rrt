#ifndef RECT_TESTS_H
#define RECT_TESTS_H

#include <gtest/gtest.h>

#include "../src/rect.h"

TEST(rect, append1) {
  rrt::Rect r1, r2;
  r1.append(10., 9.);
  r1.append(19., 20.);
  ASSERT_EQ(r1.box().min_corner().get<0>(), 10.);
  ASSERT_EQ(r1.box().min_corner().get<1>(), 9.);
  ASSERT_EQ(r1.box().max_corner().get<0>(), 19.);
  ASSERT_EQ(r1.box().max_corner().get<1>(), 20.);
  r1.append(14., 15.);
  ASSERT_EQ(r1.box().min_corner().get<0>(), 10.);
  ASSERT_EQ(r1.box().min_corner().get<1>(), 9.);
  ASSERT_EQ(r1.box().max_corner().get<0>(), 19.);
  ASSERT_EQ(r1.box().max_corner().get<1>(), 20.);
  r1.append(30., 31.);
  ASSERT_EQ(r1.box().min_corner().get<0>(), 10.);
  ASSERT_EQ(r1.box().min_corner().get<1>(), 9.);
  ASSERT_EQ(r1.box().max_corner().get<0>(), 30.);
  ASSERT_EQ(r1.box().max_corner().get<1>(), 31.);
}

TEST(rect, append2) {
  rrt::Rect r1, r2;
  r1.append(rrt::Point(4., 5., 10.));
  ASSERT_EQ(r1.box().min_corner().get<0>(), -6.);
  ASSERT_EQ(r1.box().min_corner().get<1>(), -5.);
  ASSERT_EQ(r1.box().max_corner().get<0>(), 14.);
  ASSERT_EQ(r1.box().max_corner().get<1>(), 15.);
  r1.append(rrt::Point(5., 6., 4.));
  ASSERT_EQ(r1.box().min_corner().get<0>(), -6.);
  ASSERT_EQ(r1.box().min_corner().get<1>(), -5.);
  ASSERT_EQ(r1.box().max_corner().get<0>(), 14.);
  ASSERT_EQ(r1.box().max_corner().get<1>(), 15.);
  r1.append(rrt::Point(30., 31., 10));
  ASSERT_EQ(r1.box().min_corner().get<0>(), -6.);
  ASSERT_EQ(r1.box().min_corner().get<1>(), -5.);
  ASSERT_EQ(r1.box().max_corner().get<0>(), 40.);
  ASSERT_EQ(r1.box().max_corner().get<1>(), 41.);
}

TEST(rect, intersects1) {
  rrt::Rect r1, r2;
  r1.append(10., 10.);
  r1.append(20., 20.);
  r2.append(19., 19.);
  r2.append(30., 30.);
  ASSERT_TRUE(r1.intersects(r2));
}

TEST(rect, intersects2) {
  rrt::Rect r1, r2;
  r1.append(10., 10.);
  r1.append(20., 20.);
  r2.append(21., 21.);
  r2.append(30., 30.);
  ASSERT_FALSE(r1.intersects(r2));
}

TEST(rect, intersects3) {
  rrt::Rect r1, r2;
  r1.append(10., 10.);
  r1.append(20., 20.);
  r2.append(20., 20.);
  r2.append(30., 30.);
  ASSERT_TRUE(r1.intersects(r2));
}

TEST(rect, intersects4) {
  rrt::Rect r1, r2;
  r1.append(10., 10.);
  r1.append(20., 20.);
  r2.append(19., 21.);
  r2.append(30., 30.);
  ASSERT_FALSE(r1.intersects(r2));
}

TEST(rect, intersects5) {
  rrt::Rect r1, r2;
  r1.append(10., 10.);
  r1.append(20., 20.);
  r2.append(21., 19.);
  r2.append(30., 30.);
  ASSERT_FALSE(r1.intersects(r2));
}

TEST(rect, intersects6) {
  rrt::Rect r1, r2;
  r1.append(10., 10.);
  r1.append(20., 20.);
  r2.append(5., 5.);
  r2.append(-30., -30.);
  ASSERT_FALSE(r1.intersects(r2));
}

#endif  // RECT_TESTS_H
