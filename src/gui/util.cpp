#include "util.h"

std::vector<std::string> preparePath(rrt::xmlSpatial_t spatial) {
  auto path = spatial->xmlSpatialInfo().cadastralNumber().strings();
  while (path.size() != 3) {
    path.pop_back();
  }
  path.push_back(spatial->xmlSpatialInfo().type());
  path.push_back(spatial->xmlSpatialInfo().cadastralNumber().string());
  return path;
}
