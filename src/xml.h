#ifndef XML_H
#define XML_H

#include <boost/filesystem/path.hpp>
#include <memory>
#include <pugixml.hpp>
#include <string>
#include "xmlparser.h"

namespace rrt {

class XML {
 public:
  using cadastralNumbers_t = XMLParser::cadastralNumbers_t;

  XML(const char* path);
  cadastralNumbers_t cadastralNumbers() const;

 private:
  boost::filesystem::path path_;
  std::shared_ptr<XMLParser> parser_;
  cadastralNumbers_t cadastralNumbers_;
};

pugi::xpath_node_set getCadastralNumberNodes(pugi::xml_document& root);
pugi::xpath_node_set getSpatialNodes(const pugi::xml_node& doc);
std::string localSelector(const char* input);

}  // namespace rrt

#endif  // XML_H
