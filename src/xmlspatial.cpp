#include "xmlspatial.h"

#include <boost/log/trivial.hpp>
#include <stdexcept>
#include "fmt/core.h"

namespace rrt {

XMLSpatial::XMLSpatial(const XMLSpatialInfo& info)
    : xmlSpatialInfo_(info),
      xmlInfo_(std::make_shared<XMLInfo>()),
      spatial_(std::make_shared<Spatial>()) {}

XMLSpatial::XMLSpatial(const XMLSpatialSerialized& serialized)
    : xmlSpatialInfo_(serialized.spatial_type,
                      serialized.spatial_cadastral_number),
      xmlInfo_(std::make_shared<XMLInfo>(
          serialized.xml_type,
          serialized.xml_date,
          serialized.xml_order_number,
          XMLSpatialInfo(serialized.root_type,
                         serialized.root_cadastral_number))),
      spatial_(std::make_shared<Spatial>(serialized.rect,
                                         serialized.polygons,
                                         serialized.circle_polygons,
                                         serialized.linestrings,
                                         serialized.circles)) {
  if (serialized.notFull()) {
    BOOST_LOG_TRIVIAL(warning)
        << "XMLSpatial(const XMLSpatialSerialized&): serialized is not full";
  }
}

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

const std::shared_ptr<Spatial> XMLSpatial::spatial() const {
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

XMLSpatialSerialized XMLSpatial::serialize() const {
  XMLSpatialSerialized res;

  res.spatial_type = xmlSpatialInfo().type();
  res.spatial_cadastral_number = xmlSpatialInfo().cadastralNumber().string();

  res.root_type = xmlInfo().rootSpatialInfo().type();
  res.root_cadastral_number =
      xmlInfo().rootSpatialInfo().cadastralNumber().string();

  res.xml_type = xmlInfo().type();
  res.xml_date = xmlInfo().date();
  res.xml_order_number = xmlInfo().orderNumber();

  res.rect = spatial()->serializeRect();
  res.polygons = spatial()->serializePolygons();
  res.linestrings = spatial()->serializeLinestrings();
  res.circle_polygons = spatial()->serializeCirclePolygons();
  res.circles = spatial()->serializeCircles();

  if (res.notFull()) {
    throw std::runtime_error("XMLSpatial::serialize: result is not full");
  }
  return res;
}

XMLSpatialSerialized::XMLSpatialSerialized() {}

bool XMLSpatialSerialized::notFull() const {
  return spatial_type.empty() || spatial_cadastral_number.empty() ||
         root_type.empty() || root_cadastral_number.empty() ||
         xml_type.empty() || xml_date.empty() || xml_order_number.empty();
}

void XMLSpatialSerialized::insert(char* type, char* data) {
  if (std::strcmp(type, "spatial_type") == 0) {
    spatial_type = data;
  } else if (std::strcmp(type, "spatial_cadastral_number") == 0) {
    spatial_cadastral_number = data;
  } else if (std::strcmp(type, "root_type") == 0) {
    root_type = data;
  } else if (std::strcmp(type, "root_cadastral_number") == 0) {
    root_cadastral_number = data;
  } else if (std::strcmp(type, "xml_type") == 0) {
    xml_type = data;
  } else if (std::strcmp(type, "xml_date") == 0) {
    xml_date = data;
  } else if (std::strcmp(type, "xml_order_number") == 0) {
    xml_order_number = data;
  } else if (std::strcmp(type, "rect") == 0) {
    rect = data;
  } else if (std::strcmp(type, "polygons") == 0) {
    polygons = data;
  } else if (std::strcmp(type, "linestrings") == 0) {
    linestrings = data;
  } else if (std::strcmp(type, "circle_polygons") == 0) {
    circle_polygons = data;
  } else if (std::strcmp(type, "circles") == 0) {
    circles = data;
  }
}

}  // namespace rrt
