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

struct StandardParserSettings {
  StandardParserSettings();

  std::string localSelector() const;

  std::string CadastralNumber = "CadastralNumber";
  std::string CadastralBlock = "CadastralBlock";

  std::string EntitySpatial = "EntitySpatial";
  std::string SpatialData = "SpatialData";
  std::string SpatialElement = "SpatialElement";

  std::string Ordinate = "Ordinate";
  std::string X = "X";
  std::string Y = "Y";

  std::string CertificationDoc = "CertificationDoc";
  std::string Date = "Date";
  std::string Number = "Number";
};

class StandardParser : public XMLParser {
 public:
  StandardParser(pugi::xml_document& root);
  StandardParser(pugi::xml_document& root,
                 const StandardParserSettings& settings);

  xmlSpatials_t getXMLSpatials() override;
  XMLInfo getXMLInfo() override;

 private:
  StandardParserSettings settings_;

 private:
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
