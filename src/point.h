#ifndef XMLPOINT_H
#define XMLPOINT_H

#include <boost/geometry.hpp>
#include <iostream>
#include <optional>
#include "common.h"

namespace bg = boost::geometry;

namespace rrt {

class Point {
 public:
  typedef bg::model::point<double, 2, bg::cs::cartesian> point_t;

  Point(double x, double y, std::optional<double> r);

  double x() const;
  double y() const;
  point_t point() const;
  std::optional<double> r() const;

  friend std::ostream& operator<<(std::ostream& out, const Point& rhs);
  bool operator==(const Point& rhs) {
    return x() == rhs.x() && y() == rhs.y() && r_ == rhs.r_;
  }

 private:
  point_t point_;
  std::optional<double> r_;
};

std::ostream& operator<<(std::ostream& out, const Point& rhs);

}  // namespace rrt

#endif  // XMLPOINT_H
