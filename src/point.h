#ifndef XMLPOINT_H
#define XMLPOINT_H

#include <boost/geometry.hpp>
#include <iostream>
#include <optional>
#include "compare.h"

namespace bg = boost::geometry;

namespace rrt {

class Point {
 public:
  typedef bg::model::point<double, 2, bg::cs::cartesian> point_t;

  Point(double x, double y, std::optional<double> r);
  Point(double x, double y, double r);
  Point(double x, double y);

  double x() const;
  double y() const;
  point_t point() const;
  std::optional<double> r() const;

  friend std::ostream& operator<<(std::ostream& out, const Point& rhs);
  bool operator==(const Point& rhs) {
    return compare(x(), rhs.x()) && compare(y(), rhs.y()) &&
           compare(r_.value_or(0.), rhs.r_.value_or(0.));
  }

 private:
  point_t point_;
  std::optional<double> r_;
};

std::ostream& operator<<(std::ostream& out, const Point& rhs);

}  // namespace rrt

#endif  // XMLPOINT_H
