#include "standardparser.h"

#include <fmt/core.h>
#include <boost/lexical_cast.hpp>
#include <boost/log/trivial.hpp>
#include <cstring>
#include <memory>
#include "point.h"

namespace rrt {

StandardParser::StandardParser(pugi::xml_document& root) : XMLParser(root) {}

StandardParser::StandardParser(pugi::xml_document& root,
                               const StandardParserSettings& settings)
    : XMLParser(root), settings_(settings) {}

pugi::xpath_node_set rrt::StandardParser::getCadastralNumberNodes() {
  pugi::xpath_node_set res =
      root_.select_nodes(attributeSelector(settings_.CadastralNumber).c_str());
  return res;
}

pugi::xpath_node StandardParser::getFirstCadastralNumberNode() {
  pugi::xpath_node res =
      root_.select_node(attributeSelector(settings_.CadastralNumber).c_str());
  return res;
}

pugi::xpath_node_set StandardParser::getEntitySpatialNodes(
    const pugi::xml_node& cadastralNumberNode) {
  pugi::xpath_node_set res;
  auto selector = localSelector(settings_.EntitySpatial);
  if (std::strcmp(cadastralNumberNode.name(),
                  settings_.CadastralBlock.c_str()) == 0) {
    auto spatialNode = cadastralNumberNode.child(settings_.SpatialData.c_str());
    res = spatialNode.select_nodes(selector.c_str());
  } else {
    res = cadastralNumberNode.select_nodes(selector.c_str());
  }
  return res;
}

pugi::xpath_node_set StandardParser::getSpatialElementNodes(
    const pugi::xml_node& entitySpatialNode) {
  auto selector = localSelector(settings_.SpatialElement);
  return entitySpatialNode.select_nodes(selector.c_str());
}

pugi::xpath_node_set StandardParser::getOrdinateNodes(
    const pugi::xml_node& spatialElementNode) {
  auto selector = localSelector(settings_.Ordinate);
  return spatialElementNode.select_nodes(selector.c_str());
}

XMLSpatialInfo StandardParser::getSpatialInfo(
    const pugi::xml_node& cadastralNumberNode) {
  XMLSpatialInfo res(
      cadastralNumberNode.name(),
      cadastralNumberNode.attribute(settings_.CadastralNumber.c_str()).value());
  return res;
}

Point StandardParser::getXmlPoint(const pugi::xml_node& ordinateNode) {
  auto coords = getCoordinates(ordinateNode);
  auto radius = getRadius(ordinateNode);
  return Point(coords.first, coords.second, radius);
}

std::pair<double, double> StandardParser::getCoordinates(
    const pugi::xml_node& ordinateNode) {
  return {boost::lexical_cast<double>(
              ordinateNode.attribute(settings_.X.c_str()).value()),
          boost::lexical_cast<double>(
              ordinateNode.attribute(settings_.Y.c_str()).value())};
}

std::optional<double> StandardParser::getRadius(
    const pugi::xml_node& ordinateNode) {
  auto nextSibling = ordinateNode.next_sibling();
  if (nextSibling) {
    return std::stod(nextSibling.child_value());
  }
  return std::nullopt;
}

std::vector<std::vector<std::vector<Point>>> StandardParser::getCadastralItems(
    const pugi::xml_node& cadastralNumberNode) {
  std::vector<std::vector<std::vector<Point>>> res;
  for (auto& entitySpatial : getEntitySpatialNodes(cadastralNumberNode)) {
    auto entitySpatialItems = getEntitySpatialItems(entitySpatial.node());
    res.push_back(entitySpatialItems);
  }
  BOOST_LOG_TRIVIAL(debug) << "StandardParser::getCadastralItems: got "
                           << res.size() << " CadastralItems";
  return res;
}

std::vector<std::vector<Point>> StandardParser::getEntitySpatialItems(
    const pugi::xml_node& entitySpatialNode) {
  std::vector<std::vector<Point>> res;
  for (auto& spatialElement : getSpatialElementNodes(entitySpatialNode)) {
    auto spatialElementItems = getSpatialElementItems(spatialElement.node());
    res.push_back(spatialElementItems);
  }
  BOOST_LOG_TRIVIAL(debug) << "StandardParser::getEntitySpatialItems: got "
                           << res.size() << " EntitySpatialItems";
  return res;
}

std::vector<Point> StandardParser::getSpatialElementItems(
    const pugi::xml_node& spatialElementNode) {
  std::vector<Point> res;
  for (auto& ordinate : getOrdinateNodes(spatialElementNode)) {
    Point p = getXmlPoint(ordinate.node());
    res.push_back(p);
    BOOST_LOG_TRIVIAL(debug) << p;
  }
  BOOST_LOG_TRIVIAL(debug) << "StandardParser::getSpatialElementItems: got "
                           << res.size() << " SpatialElementItems";
  return res;
}

XMLParser::xmlSpatials_t StandardParser::getXMLSpatials() {
  xmlSpatials_t res;
  for (auto& cadastral : getCadastralNumberNodes()) {
    auto spatialInfo = getSpatialInfo(cadastral.node());
    auto xmlSpatial = std::make_shared<XMLSpatial>(spatialInfo);
    auto cadastralItems = getCadastralItems(cadastral.node());
    BOOST_LOG_TRIVIAL(debug)
        << "StandardParser::getXMLSpatials: got: " << cadastralItems.size()
        << " CadastralItems";
    xmlSpatial->spatial()->append(cadastralItems);
    res.push_back(xmlSpatial);
    BOOST_LOG_TRIVIAL(info)
        << "StandardParser::getXMLSpatials: got: " << spatialInfo;
  }
  return res;
}

XMLInfo StandardParser::getXMLInfo() {
  auto firstChild = *root_.children().begin();
  std::string type(firstChild.name());

  auto certDocNode =
      root_.select_node(childSelector(settings_.CertificationDoc).c_str())
          .node();
  auto dateNode =
      certDocNode.select_node(localSelector(settings_.Date).c_str()).node();
  auto numberNode =
      certDocNode.select_node(localSelector(settings_.Number).c_str()).node();

  if (type.empty() || certDocNode.empty() || dateNode.empty() ||
      numberNode.empty()) {
    throw(std::runtime_error("StandardParser::getXMLInfo: bad XML file"));
  }

  std::string date(dateNode.child_value());
  std::string number(numberNode.child_value());

  auto firstNode = getFirstCadastralNumberNode().node();
  auto firstSpatialInfo = getSpatialInfo(firstNode);

  BOOST_LOG_TRIVIAL(info) << "StandardParser::getXMLInfo: got type: " << type
                          << ", date: " << date << ", number: " << number;

  return XMLInfo(type, date, number, firstSpatialInfo);
}

StandardParserSettings::StandardParserSettings() {}

}  // namespace rrt
