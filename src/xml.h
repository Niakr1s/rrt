#ifndef XML_H
#define XML_H

#include <boost/filesystem/path.hpp>
#include <memory>
#include "xmlparser.h"

namespace rrt {

class XML {
 public:
  using xmlSpatials_t = XMLParser::xmlSpatials_t;

  XML(const char* path);
  const xmlSpatials_t& xmlSpatials() const;
  xmlSpatials_t& xmlSpatials();

 private:
  boost::filesystem::path path_;
  std::shared_ptr<XMLParser> parser_;
  xmlSpatials_t spatials_;
};

}  // namespace rrt

#endif  // XML_H
