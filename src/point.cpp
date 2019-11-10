#include "point.h"

#include <fmt/core.h>

namespace rrt {

Point::Point(double x, double y, std::optional<double> r)
    : point_(x, y), r_(r) {}

double Point::x() const {
  return point_.get<0>();
}

double Point::y() const {
  return point_.get<1>();
}

std::optional<double> Point::r() const {
  return r_;
}

Point::point_t Point::point() const {
  return point_;
}

std::ostream& operator<<(std::ostream& out, const Point& rhs) {
  auto str = fmt::format("X={:.2f}, Y={:.2f}", rhs.x(), rhs.y());
  if (rhs.r_.has_value()) {
    str.append(fmt::format(", R={:.2f}", rhs.r_.value()));
  }
  return out << str;
}

}  // namespace rrt
