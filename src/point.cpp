#include "point.h"

#include <fmt/core.h>
#include <sstream>
#include "compare.h"

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

std::string Point::serialize(const std::vector<Point>& points) {
  if (points.empty()) {
    return "empty";
  }
  std::ostringstream ss;
  for (auto& p : points) {
    ss << std::setprecision(10) << p << ",";
  }
  return ss.str();
}

std::vector<Point> Point::deserialize(const std::string& in) {
  std::vector<Point> res;
  if (in == "empty") {
    return res;
  }
  std::istringstream ss(in);
  double x, y, r;
  while (ss >> x >> y >> r) {
    res.push_back(compare(r, 0.) ? Point(x, y) : Point(x, y, r));
    ss.get();  // consuming ","
  }
  return res;
}

Point::point_t Point::point() const {
  return point_;
}

std::ostream& operator<<(std::ostream& out, const Point& rhs) {
  return out << rhs.x() << " " << rhs.y() << " " << rhs.r_.value_or(0.);
}

}  // namespace rrt
