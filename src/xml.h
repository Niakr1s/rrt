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
  XML(const std::string& path);
  XML(const std::wstring& path);
  const xmlSpatials_t& xmlSpatials() const;
  xmlSpatials_t& xmlSpatials();

  void saveToDXF(std::string path = "",
                 DXF::Version version = DXF::Version::AC1021);
  bf::path renameFile();
  void copyFile(bf::path newPath);

  const XMLInfo& xmlInfo() const;

  xmlSpatials_t intersects(const Spatial& spatial) const;

  bf::path path() const;

 private:
  std::shared_ptr<XMLInfo> xmlInfo_;
  std::shared_ptr<XMLParser> parser_;
  xmlSpatials_t spatials_;

 private:
  void addXmlInfoToSpatials();
};

}  // namespace rrt

#endif  // XML_H
