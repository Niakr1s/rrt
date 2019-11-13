#include "rect.h"

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
    if (x > box_.max_corner().get<0>()) {
      box_.max_corner().set<0>(x);
    } else if (x < box_.min_corner().get<0>()) {
      box_.min_corner().set<0>(x);
    }
    if (y > box_.max_corner().get<1>()) {
      box_.max_corner().set<1>(y);
    } else if (y < box_.min_corner().get<1>()) {
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

Rect::box_t Rect::box() const {
  return box_;
}

}  // namespace rrt
