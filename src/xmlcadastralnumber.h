#ifndef XMLCADASTRALNUMBER_H
#define XMLCADASTRALNUMBER_H

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include "common.h"
#include "xmlcadastralnumberinfo.h"
#include "xmlpoint.h"

namespace rrt {

class XMLCadastralNumber {
 public:
  static bg::strategy::buffer::point_circle point_strategy;
  static bg::strategy::buffer::join_round join_strategy;
  static bg::strategy::buffer::end_round end_strategy;
  static bg::strategy::buffer::side_straight side_strategy;

  XMLCadastralNumber(const XMLCadastralNumberInfo& info);

  XMLCadastralNumberInfo info() const;

  template <typename Geometry>
  bool intersects(const Geometry& geometry) const {
    if (bg::intersects(polygons_, geometry)) {
      return true;
    }
    if (bg::intersects(linestrings_, geometry)) {
      return true;
    }
    for (auto& circle : circles_) {
      if (bg::intersects(circle, geometry)) {
        return true;
      }
    }
    return false;
  };

  void append(std::vector<std::vector<std::vector<XMLPoint>>>& xmlPoints);
  static bool isClosed(std::vector<XMLPoint>& contour);
  static bool isCircle(std::vector<XMLPoint>& contour);
  static bool isClosed(std::vector<std::vector<XMLPoint>>& polygon);

 private:
  XMLCadastralNumberInfo info_;
  mpolygon_t polygons_;
  mlinestring_t linestrings_;
  std::vector<mpolygon_t> circles_;
  mpoint_t circlePoints_;
};

}  // namespace rrt

#endif  // XMLCADASTRALNUMBER_H
