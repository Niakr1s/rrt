#ifndef STANDARDPARSER_H
#define STANDARDPARSER_H

#include <optional>
#include <pugixml.hpp>
#include <string>
#include <vector>
#include "xmlparser.h"
#include "xmlpoint.h"
#include "xmlcadastralnumberinfo.h"

namespace rrt {

class StandardParser : public XMLParser {
 public:
  StandardParser(pugi::xml_document& root);

  cadastralNumbers_t getCadastralNumbers() override;

  pugi::xpath_node_set getCadastralNumberNodes();
  pugi::xpath_node_set getEntitySpatialNodes(
      const pugi::xml_node& cadastralNumberNode);
  pugi::xpath_node_set getSpatialElementNodes(
      const pugi::xml_node& entitySpatialNode);
  pugi::xpath_node_set getOrdinateNodes(
      const pugi::xml_node& spatialElementNode);
  XMLCadastralNumberInfo getSpatialInfo(const pugi::xml_node& cadastralNumberNode);

  std::pair<double, double> getCoordinates(const pugi::xml_node& ordinateNode);
  std::optional<double> getRadius(const pugi::xml_node& ordinateNode);

  std::vector<std::vector<std::vector<XMLPoint>>> getCadastralItems(
      const pugi::xml_node& cadastralNumberNode);
  std::vector<std::vector<XMLPoint>> getEntitySpatialItems(
      const pugi::xml_node& entitySpatialNode);
  std::vector<XMLPoint> getSpatialElementItems(
      const pugi::xml_node& spatialElementNode);
  XMLPoint getXmlPoint(const pugi::xml_node& ordinateNode);
};

}  // namespace rrt

#endif  // STANDARDPARSER_H
