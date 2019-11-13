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

DXF::Color XMLSpatial::color() const {
  DXF::Color res = DXF::Color::GREY;
  if (info().type() == "Parcel") {
    res = DXF::Color::GREEN;
  } else if (info().type() == "Building" || info().type() == "Construction" ||
             info().type() == "Uncompleted_Construction") {
    res = DXF::Color::RED;
  } else if (info().type() == "CadastralBlock") {
    res = DXF::Color::BLACK;
  }
  return res;
}

}  // namespace rrt
