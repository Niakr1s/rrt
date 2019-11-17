#include "rect.h"

#include <sstream>
#include "compare.h"

namespace rrt {

Rect::Rect() : empty_(true) {}

void Rect::append(double x, double y, std::optional<double> r) {
  if (r.has_value()) {
    append(x, y, r.value());
  } else {
    append(x, y);
  }
}

void Rect::append(double x, double y) {
  if (empty()) {
    box_.min_corner() = {x, y};
    box_.max_corner() = {x, y};
    empty_ = false;
  } else {
    if (x > xmax()) {
      box_.max_corner().set<0>(x);
    } else if (x < xmin()) {
      box_.min_corner().set<0>(x);
    }
    if (y > ymax()) {
      box_.max_corner().set<1>(y);
    } else if (y < ymin()) {
      box_.min_corner().set<1>(y);
    }
  }
}

void Rect::append(double x, double y, double r) {
  append(x + r, y + r);
  append(x - r, y - r);
}

void Rect::append(const Point& point) {
  append(point.x(), point.y(), point.r());
}

void Rect::append(const Rect::point_t& point) {
  append(point.get<0>(), point.get<1>());
}

Rect::point_t Rect::centroid() const {
  point_t res;
  bg::centroid(box_, res);
  return res;
}

bool Rect::empty() const {
  return empty_;
}

bool Rect::intersects(const Rect& rhs) const {
  return bg::intersects(box_, rhs.box_);
}

bool Rect::operator==(const Rect& rhs) const {
  return empty_ == rhs.empty_ && compare(xmin(), rhs.xmin()) &&
         compare(ymin(), rhs.ymin()) && compare(xmax(), rhs.xmax()) &&
         compare(ymax(), rhs.ymax());
}

double Rect::xmin() const {
  return box_.min_corner().get<0>();
}

double Rect::ymin() const {
  return box_.min_corner().get<1>();
}

double Rect::xmax() const {
  return box_.max_corner().get<0>();
}

double Rect::ymax() const {
  return box_.max_corner().get<1>();
}

Rect::box_t Rect::box() const {
  return box_;
}

std::string Rect::serialize() const {
  if (empty()) {
    return "empty";
  }
  std::ostringstream ss;
  ss << xmin() << " " << ymin() << " " << xmax() << " " << ymax();
  return ss.str();
}

Rect Rect::deserialize(const std::string& in) {
  if (in == "empty") {
    return Rect();
  }
  std::istringstream ss(in);
  Rect r;
  double xmin, ymin, xmax, ymax;
  ss >> xmin >> ymin >> xmax >> ymax;
  r.append(xmin, ymin);
  r.append(xmax, ymax);
  return r;
}

}  // namespace rrt
