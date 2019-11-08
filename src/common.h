#ifndef COMMON_H
#define COMMON_H

#include <boost/geometry.hpp>

namespace bg = boost::geometry;

typedef bg::model::point<double, 2, bg::cs::cartesian> point_t;
typedef bg::model::multi_point<point_t> mpoint_t;
typedef bg::model::polygon<point_t> polygon_t;
typedef bg::model::multi_polygon<polygon_t> mpolygon_t;
typedef bg::model::linestring<point_t> linestring_t;
typedef bg::model::multi_linestring<linestring_t> mlinestring_t;

#endif  // COMMON_H
