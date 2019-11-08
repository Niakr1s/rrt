#include "xmlcadastralnumber.h"

namespace rrt {

XMLCadastralNumber::XMLCadastralNumber(const XMLCadastralNumberInfo& info)
    : info_(info) {}

XMLCadastralNumberInfo XMLCadastralNumber::info() const {
  return info_;
}

void XMLCadastralNumber::append(
    std::vector<std::vector<std::vector<XMLPoint>>>& xmlPoints) {
  for (auto& polygon : xmlPoints) {
    if (isClosed(polygon)) {
      polygon_t polygonToPush;
      for (size_t i = 0; i != polygon.size(); ++i) {
        if (i == 0) {
          for (auto& p : polygon[i]) {
            bg::append(polygonToPush.outer(), p.point());
          }
        } else {
          auto newSize = polygonToPush.inners().size() + 1;
          polygonToPush.inners().resize(newSize);
          for (auto& p : polygon[i]) {
            bg::append(polygonToPush.inners()[newSize - 1], p.point());
          }
        }
      }
      bg::correct(polygonToPush);
      polygons_.push_back(polygonToPush);
    } else {
      for (size_t i = 0; i != polygon.size(); ++i) {
        if (isClosed(polygon[i])) {
          polygon_t polygonToPush;
          for (auto& p : polygon[i]) {
            bg::append(polygonToPush.outer(), p.point());
          }
          polygons_.push_back(polygonToPush);
        } else if (isCircle(polygon[i])) {
          auto circle = polygon[i][0];
          mpolygon_t mPolygonToPush;
          bg::strategy::buffer::distance_symmetric<double> distance_strategy(
              circle.r().value());
          bg::buffer(circle.point(), mPolygonToPush, distance_strategy,
                     side_strategy, join_strategy, end_strategy,
                     point_strategy);
          circles_.push_back(mPolygonToPush);
          bg::append(circlePoints_, circle.point());
        } else {
          linestring_t polygonToPush;
          for (auto& p : polygon[i]) {
            bg::append(polygonToPush, p.point());
          }
          linestrings_.push_back(polygonToPush);
        }
      }
    }
  }
}

bool XMLCadastralNumber::isClosed(std::vector<XMLPoint>& contour) {
  return contour.size() > 1 && (contour.front() == contour.back());
}

bool XMLCadastralNumber::isCircle(std::vector<XMLPoint>& contour) {
  return contour.size() == 1 && contour[0].r().has_value();
}

bool XMLCadastralNumber::isClosed(std::vector<std::vector<XMLPoint>>& polygon) {
  for (auto& contour : polygon) {
    if (!isClosed(contour)) {
      return false;
    }
  }
  return true;
};

bg::strategy::buffer::point_circle XMLCadastralNumber::point_strategy(90);
bg::strategy::buffer::side_straight XMLCadastralNumber::side_strategy;
bg::strategy::buffer::join_round XMLCadastralNumber::join_strategy;
bg::strategy::buffer::end_round XMLCadastralNumber::end_strategy;

}  // namespace rrt
