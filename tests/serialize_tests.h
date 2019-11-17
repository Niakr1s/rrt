#ifndef SERIALIZE_TESTS_H
#define SERIALIZE_TESTS_H

#include <gtest/gtest.h>

#include "point.h"
#include "rect.h"

TEST(serialize, rect) {
  rrt::Rect r;
  r.append(10.555, 10.789);
  r.append(20.1234, 20.5646);
  auto serStr = r.serialize();
  ASSERT_EQ(serStr, "10.555 10.789 20.1234 20.5646");
  auto r1 = rrt::Rect::deserialize(serStr);
  ASSERT_EQ(r, r1);
}

TEST(serialize, points) {
  std::vector<rrt::Point> vec{{1.876243, 2.4545, 3.33456},
                              {3.453412, -10.21235, 0.000},
                              {3.453412, -10.21235},
                              {100.11123, -1000.2323, 5000.222}};
  auto ser = rrt::Point::serialize(vec);
  ASSERT_EQ(ser,
            "1.876243 2.4545 3.33456,3.453412 -10.21235 0,3.453412 -10.21235 0,"
            "100.11123 -1000.2323 5000.222,");
  auto vec1 = rrt::Point::deserialize(ser);
  ASSERT_EQ(vec1, vec);
}

#endif  // SERIALIZE_TESTS_H
