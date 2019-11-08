#include "xmlparser.h"

#include "standardparser.h"

namespace rrt {

XMLParser::XMLParser(pugi::xml_document& root) : root_(root) {}

std::shared_ptr<XMLParser> XMLParser::chooseParser(pugi::xml_document& root) {
  return std::make_shared<StandardParser>(root);
}

std::string XMLParser::localSelector(const char* input) {
  std::string res = ".//*[local-name()='";
  res.append(input);
  res.append("']");
  return res;
}

}  // namespace rrt
