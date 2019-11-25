#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <memory>
#include <pugixml.hpp>
#include <vector>
#include "xmlinfo.h"
#include "xmlspatial.h"

namespace rrt {

class XMLParser {
 public:
  XMLParser(pugi::xml_document& root);
  virtual ~XMLParser() = default;

  virtual xmlSpatials_t getXMLSpatials() = 0;
  virtual XMLInfo getXMLInfo() = 0;

  static std::shared_ptr<XMLParser> chooseParser(pugi::xml_document& root);

  static const std::string localSelector(const std::string& input);
  static const std::string attributeSelector(const std::string& input);
  static const std::string childSelector(const std::string& input);

 protected:
  pugi::xml_document& root_;
};

}  // namespace rrt

#endif  // XMLPARSER_H
