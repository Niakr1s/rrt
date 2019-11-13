#include "xmlparser.h"

#include "standardparser.h"

namespace rrt {

XMLParser::XMLParser(pugi::xml_document& root) : root_(root) {}

std::shared_ptr<XMLParser> XMLParser::chooseParser(pugi::xml_document& root) {
  return std::make_shared<StandardParser>(root);
}

const std::string XMLParser::localSelector(const std::string& input) {
  std::string res = ".//*[local-name()='";
  res.append(input);
  res.append("']");
  return res;
}

const std::string XMLParser::attributeSelector(const std::string& input) {
  std::string res = ".//*[@";
  res.append(input);
  res.append("]");
  return res;
}

const std::string XMLParser::childSelector(const std::string& input) {
  std::string res = ".//";
  res.append(input);
  return res;
}

}  // namespace rrt
