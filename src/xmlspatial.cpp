#include "xmlspatial.h"

namespace rrt {

XMLSpatial::XMLSpatial(const XMLSpatialInfo& info)
    : xmlSpatialInfo_(info),
      xmlInfo_(std::make_shared<XMLInfo>()),
      spatial_(std::make_shared<Spatial>()) {}

XMLSpatialInfo XMLSpatial::xmlSpatialInfo() const {
  return xmlSpatialInfo_;
}

void XMLSpatial::add(std::shared_ptr<XMLInfo> xmlInfo) {
  xmlInfo_ = xmlInfo;
}

XMLInfo XMLSpatial::xmlInfo() const {
  return *xmlInfo_;
}

std::shared_ptr<Spatial> XMLSpatial::spatial() {
  return spatial_;
}

DXF::Color XMLSpatial::color() const {
  DXF::Color res = DXF::Color::GREY;
  if (xmlSpatialInfo().type() == "Parcel") {
    res = DXF::Color::GREEN;
  } else if (xmlSpatialInfo().type() == "Building" ||
             xmlSpatialInfo().type() == "Construction" ||
             xmlSpatialInfo().type() == "Uncompleted_Construction") {
    res = DXF::Color::RED;
  } else if (xmlSpatialInfo().type() == "CadastralBlock") {
    res = DXF::Color::BLACK;
  }
  return res;
}

}  // namespace rrt
