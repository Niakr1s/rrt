#include "xmlspatial.h"

namespace rrt {

XMLSpatial::XMLSpatial(const XMLSpatialInfo& info)
    : info_(info), spatial_(std::make_shared<Spatial>()) {}

XMLSpatialInfo XMLSpatial::info() const {
  return info_;
}

std::shared_ptr<Spatial> XMLSpatial::spatial() {
  return spatial_;
}

}  // namespace rrt
