#include "xmlspatial.h"

namespace rrt {

XMLSpatial::XMLSpatial(const XMLSpatialInfo& info) : info_(info) {}

XMLSpatialInfo XMLSpatial::info() const {
  return info_;
}

Spatial& XMLSpatial::spatial() {
  return spatial_;
}

const Spatial& XMLSpatial::spatial() const {
  return spatial_;
}

}  // namespace rrt
