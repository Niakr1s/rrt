#ifndef RECT_H
#define RECT_H

#include <boost/geometry.hpp>
#include <optional>
#include "point.h"

namespace bg = boost::geometry;

namespace rrt {

class Rect {
 public:
  typedef Point::point_t point_t;
  typedef bg::model::box<point_t> box_t;

  Rect();

  void append(double x, double y, std::optional<double> r);
  void append(double x, double y);
  void append(double x, double y, double r);
  void append(const Point& point);
  void append(const point_t& point);

  point_t centroid() const;

  bool empty() const;
  box_t box() const;
  bool intersects(const Rect& rhs) const;

  bool operator==(const Rect& rhs) const;

  double xmin() const;
  double ymin() const;
  double xmax() const;
  double ymax() const;

  std::string serialize() const;
  static Rect deserialize(const std::string& in);

 private:
  bool empty_;
  box_t box_;
};

}  // namespace rrt

#endif  // RECT_H
