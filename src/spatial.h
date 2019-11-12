#ifndef SPATIAL_H
#define SPATIAL_H

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include <boost/log/trivial.hpp>
#include "point.h"
#include "rect.h"

namespace bg = boost::geometry;

namespace rrt {

class Spatial {
  friend class DXF;

 public:
  typedef Point::point_t point_t;
  typedef bg::model::multi_point<point_t> mpoint_t;
  typedef bg::model::polygon<point_t> polygon_t;
  typedef bg::model::multi_polygon<polygon_t> mpolygon_t;
  typedef bg::model::linestring<point_t> linestring_t;
  typedef bg::model::multi_linestring<linestring_t> mlinestring_t;
  typedef bg::model::box<point_t> box_t;

  static bg::strategy::buffer::point_circle point_strategy;
  static bg::strategy::buffer::join_round join_strategy;
  static bg::strategy::buffer::end_round end_strategy;
  static bg::strategy::buffer::side_straight side_strategy;

  Spatial();

  template <typename Geometry>
  bool intersects(const Geometry& geometry) const {
    if (!bg::intersects(rect_.box(), geometry)) {
      return false;
    }
    if (bg::intersects(polygons_, geometry)) {
      return true;
    }
    if (bg::intersects(linestrings_, geometry)) {
      return true;
    }
    if (bg::intersects(circlePolygons_, geometry)) {
      return true;
    }
    return false;
  }

  void append(const std::vector<std::vector<std::vector<Point>>>& points);
  void append(const std::vector<std::vector<Point>>& points);
  void append(const std::vector<Point>& points);
  void append(const Point& points);
  static bool isClosed(const std::vector<Point>& contour);
  static bool isCircle(const std::vector<Point>& contour);
  static bool isClosed(const std::vector<std::vector<Point>>& polygon);

  void updateRect(const std::vector<std::vector<std::vector<Point>>>& polygons);

 private:
  Rect rect_;
  mpolygon_t polygons_;
  mpolygon_t circlePolygons_;
  mlinestring_t linestrings_;

  // member circles_ is just for printing to dxf etc,
  // actually circles are stored in circlePolygons_
  std::vector<Point> circles_;
};

}  // namespace rrt

#endif  // SPATIAL_H
