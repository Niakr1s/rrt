#ifndef STANDARDPARSER_H
#define STANDARDPARSER_H

#include <optional>
#include <pugixml.hpp>
#include <string>
#include <vector>
#include "point.h"
#include "xmlinfo.h"
#include "xmlparser.h"
#include "xmlspatialinfo.h"

namespace rrt {

class StandardParser : public XMLParser {
 public:
  StandardParser(pugi::xml_document& root);

  xmlSpatials_t getXMLSpatials() override;
  XMLInfo getXMLInfo() override;

 private:
  static const pugi::xpath_query CADASTRAL_NUMBER_QUERY;
  pugi::xpath_node_set getCadastralNumberNodes();
  pugi::xpath_node getFirstCadastralNumberNode();
  pugi::xpath_node_set getEntitySpatialNodes(
      const pugi::xml_node& cadastralNumberNode);
  pugi::xpath_node_set getSpatialElementNodes(
      const pugi::xml_node& entitySpatialNode);
  pugi::xpath_node_set getOrdinateNodes(
      const pugi::xml_node& spatialElementNode);
  XMLSpatialInfo getSpatialInfo(const pugi::xml_node& cadastralNumberNode);

  std::pair<double, double> getCoordinates(const pugi::xml_node& ordinateNode);
  std::optional<double> getRadius(const pugi::xml_node& ordinateNode);

  std::vector<std::vector<std::vector<Point>>> getCadastralItems(
      const pugi::xml_node& cadastralNumberNode);
  std::vector<std::vector<Point>> getEntitySpatialItems(
      const pugi::xml_node& entitySpatialNode);
  std::vector<Point> getSpatialElementItems(
      const pugi::xml_node& spatialElementNode);
  Point getXmlPoint(const pugi::xml_node& ordinateNode);
};

}  // namespace rrt

#endif  // STANDARDPARSER_H
