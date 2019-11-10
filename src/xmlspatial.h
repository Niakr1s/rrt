#ifndef XMLSPATIAL_H
#define XMLSPATIAL_H

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include "common.h"
#include "spatial.h"
#include "xmlspatialinfo.h"

namespace rrt {

class XMLSpatial {
 public:
  XMLSpatial(const XMLSpatialInfo& info);

  XMLSpatialInfo info() const;

  Spatial &spatial();
  const Spatial &spatial() const;

 private:
  XMLSpatialInfo info_;
  Spatial spatial_;
};

}  // namespace rrt

#endif  // XMLSPATIAL_H
