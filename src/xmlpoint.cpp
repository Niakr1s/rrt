#include "xmlpoint.h"

#include <fmt/core.h>

namespace rrt {

XMLPoint::XMLPoint(double x, double y, std::optional<double> r)
    : point_(x, y), r_(r) {}

double XMLPoint::x() const {
  return point_.get<0>();
}

double XMLPoint::y() const {
  return point_.get<1>();
}

std::optional<double> XMLPoint::r() const {
  return r_;
}

point_t XMLPoint::point() const {
  return point_;
}

std::ostream& operator<<(std::ostream& out, const XMLPoint& rhs) {
  auto str = fmt::format("X={:.2f}, Y={:.2f}", rhs.x(), rhs.y());
  if (rhs.r_.has_value()) {
    str.append(fmt::format(", R={:.2f}", rhs.r_.value()));
  }
  return out << str;
}

}  // namespace rrt
