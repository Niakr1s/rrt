#ifndef XML_H
#define XML_H

#include <boost/filesystem/path.hpp>
#include <memory>
#include <string>
#include "xmlinfo.h"
#include "xmlparser.h"

namespace bf = boost::filesystem;

namespace rrt {

class XML {
 public:
  using xmlSpatials_t = XMLParser::xmlSpatials_t;

  XML(const std::string& path);
  const xmlSpatials_t& xmlSpatials() const;
  xmlSpatials_t& xmlSpatials();

  void saveToDXF(std::string path = "");

 private:
  bf::path path_;
  XMLInfo xmlInfo_;
  std::shared_ptr<XMLParser> parser_;
  xmlSpatials_t spatials_;
};

}  // namespace rrt

#endif  // XML_H
