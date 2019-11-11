#include "point.h"

#include <fmt/core.h>

namespace rrt {

Point::Point(double x, double y, std::optional<double> r)
    : point_(x, y), r_(r) {}

Point::Point(double x, double y, double r) : point_(x, y), r_(r) {
  if (r == 0.) {
    r_ = std::nullopt;
  }
}

Point::Point(double x, double y) : point_(x, y) {}

double Point::x() const {
  return point_.get<0>();
}

double Point::y() const {
  return point_.get<1>();
}

std::optional<double> Point::r() const {
  return r_;
}

bool Point::operator==(const Point& rhs) const {
  return compare(x(), rhs.x()) && compare(y(), rhs.y()) &&
         compare(r_.value_or(0.), rhs.r_.value_or(0.));
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
