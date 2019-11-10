#ifndef SPATIAL_H
#define SPATIAL_H

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include <boost/log/trivial.hpp>
#include "common.h"
#include "point.h"

namespace bg = boost::geometry;

namespace rrt {

class Spatial {
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
    if (bg::intersects(polygons_, geometry)) {
      BOOST_LOG_TRIVIAL(debug) << "Intersects polygons";
      return true;
    }
    if (bg::intersects(linestrings_, geometry)) {
      BOOST_LOG_TRIVIAL(debug) << "Intersects linestrings";
      return true;
    }
    return false;
  }

  void append(std::vector<std::vector<std::vector<Point>>>& points);
  static bool isClosed(std::vector<Point>& contour);
  static bool isCircle(std::vector<Point>& contour);
  static bool isClosed(std::vector<std::vector<Point>>& polygon);

 private:
  mpolygon_t polygons_;
  mlinestring_t linestrings_;
  mpoint_t circles_;
};

}  // namespace rrt

#endif  // SPATIAL_H
