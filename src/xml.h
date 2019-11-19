#ifndef XML_H
#define XML_H

#include <boost/filesystem/path.hpp>
#include <memory>
#include <string>
#include "dxf.h"
#include "xmlinfo.h"
#include "xmlparser.h"
#include "xmlspatial.h"

namespace bf = boost::filesystem;

namespace rrt {

class XML {
 public:
  using xmlSpatials_t = XMLSpatial::xmlSpatials_t;

  XML(const std::string& path);
  const xmlSpatials_t& xmlSpatials() const;
  xmlSpatials_t& xmlSpatials();

  void saveToDXF(std::string path = "",
                 DXF::Version version = DXF::Version::AC1021);
  void renameFile();

  const XMLInfo& xmlInfo() const;

  xmlSpatials_t intersects(const Spatial& spatial) const;

 private:
  bf::path path_;
  std::shared_ptr<XMLInfo> xmlInfo_;
  std::shared_ptr<XMLParser> parser_;
  xmlSpatials_t spatials_;

 private:
  void addXmlInfoToSpatials();
};

}  // namespace rrt

#endif  // XML_H
