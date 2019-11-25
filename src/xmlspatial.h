#ifndef XMLSPATIAL_H
#define XMLSPATIAL_H

#include <memory>
#include <vector>
#include "dxf.h"
#include "spatial.h"
#include "string"
#include "xmlinfo.h"
#include "xmlspatialinfo.h"

namespace rrt {

struct XMLSpatialSerialized {
  XMLSpatialSerialized();

  bool notFull() const;

  void insert(char* type, char* data);

  std::string spatial_type;
  std::string spatial_cadastral_number;

  std::string root_type;
  std::string root_cadastral_number;

  std::string xml_type;
  std::string xml_date;
  std::string xml_order_number;

  std::string rect;
  std::string polygons;
  std::string linestrings;
  std::string circle_polygons;
  std::string circles;
};

class XMLSpatial {
 public:
  XMLSpatial(const XMLSpatialInfo& xmlSpatialInfo);
  XMLSpatial(const XMLSpatialSerialized& serialized);

  XMLSpatialInfo xmlSpatialInfo() const;

  void add(std::shared_ptr<XMLInfo> xmlInfo);
  XMLInfo xmlInfo() const;

  std::shared_ptr<Spatial> spatial();
  const std::shared_ptr<Spatial> spatial() const;

  DXF::Color color() const;

  XMLSpatialSerialized serialize() const;

 private:
  XMLSpatialInfo xmlSpatialInfo_;
  std::shared_ptr<XMLInfo> xmlInfo_;
  std::shared_ptr<Spatial> spatial_;
};

typedef std::vector<std::shared_ptr<XMLSpatial>> xmlSpatials_t;

}  // namespace rrt

#endif  // XMLSPATIAL_H
