#include "spatial.h"

namespace rrt {

Spatial::Spatial() {}

void Spatial::append(std::vector<std::vector<std::vector<Point>>>& points) {
  for (auto& polygon : points) {
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
          circles_.push_back(circle);
          circlePolygons_.push_back(mPolygonToPush[0]);
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

bool Spatial::isClosed(std::vector<Point>& contour) {
  return contour.size() > 1 && (contour.front() == contour.back());
}

bool Spatial::isCircle(std::vector<Point>& contour) {
  return contour.size() == 1 && contour[0].r().has_value();
}

bool Spatial::isClosed(std::vector<std::vector<Point>>& polygon) {
  for (auto& contour : polygon) {
    if (!isClosed(contour)) {
      return false;
    }
  }
  return true;
};

bg::strategy::buffer::point_circle Spatial::point_strategy(90);
bg::strategy::buffer::side_straight Spatial::side_strategy;
bg::strategy::buffer::join_round Spatial::join_strategy;
bg::strategy::buffer::end_round Spatial::end_strategy;

}  // namespace rrt
