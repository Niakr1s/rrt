#ifndef XMLSPATIAL_H
#define XMLSPATIAL_H

#include <memory>
#include "spatial.h"
#include "xmlspatialinfo.h"

namespace rrt {

class XMLSpatial {
 public:
  XMLSpatial(const XMLSpatialInfo& info);

  XMLSpatialInfo info() const;

  std::shared_ptr<Spatial> spatial();

  DXF::Color color() const;

 private:
  XMLSpatialInfo info_;
  std::shared_ptr<Spatial> spatial_;
};

}  // namespace rrt

#endif  // XMLSPATIAL_H
