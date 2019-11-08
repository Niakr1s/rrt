#ifndef XMLPOINT_H
#define XMLPOINT_H

#include <boost/geometry.hpp>
#include <iostream>
#include <optional>
#include "common.h"

namespace rrt {

class XMLPoint {
 public:
  XMLPoint(double x, double y, std::optional<double> r);

  double x() const;
  double y() const;
  point_t point() const;
  std::optional<double> r() const;

  friend std::ostream& operator<<(std::ostream& out, const XMLPoint& rhs);
  bool operator==(const XMLPoint& rhs) {
    return x() == rhs.x() && y() == rhs.y() && r_ == rhs.r_;
  }

 private:
  point_t point_;
  std::optional<double> r_;
};

std::ostream& operator<<(std::ostream& out, const XMLPoint& rhs);

}  // namespace rrt

#endif  // XMLPOINT_H
