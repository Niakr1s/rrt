#ifndef XMLSPATIAL_H
#define XMLSPATIAL_H

#include <memory>
#include "dxf.h"
#include "spatial.h"
#include "xmlspatialinfo.h"

namespace rrt {

class XMLSpatial {
 public:
  typedef std::vector<std::shared_ptr<XMLSpatial>> xmlSpatials_t;

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
