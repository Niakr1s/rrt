#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <memory>
#include <pugixml.hpp>
#include <vector>
#include "xmlspatial.h"

namespace rrt {

class XMLParser {
 public:
  typedef std::vector<XMLSpatial> xmlSpatials_t;

  XMLParser(pugi::xml_document& root);
  virtual ~XMLParser() = default;

  virtual xmlSpatials_t getXMLSpatials() = 0;

  static std::shared_ptr<XMLParser> chooseParser(pugi::xml_document& root);
  static std::string localSelector(const char* input);

 protected:
  pugi::xml_document& root_;
};

}  // namespace rrt

#endif  // XMLPARSER_H
