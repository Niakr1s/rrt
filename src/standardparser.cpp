#include "standardparser.h"

#include <fmt/core.h>
#include <boost/lexical_cast.hpp>
#include <boost/log/trivial.hpp>
#include <cstring>
#include "point.h"

namespace rrt {

StandardParser::StandardParser(pugi::xml_document& root) : XMLParser(root) {}

pugi::xpath_node_set rrt::StandardParser::getCadastralNumberNodes() {
  static const pugi::xpath_query CADASTRAL_NUMBER_QUERY(
      ".//*[@CadastralNumber]");
  pugi::xpath_node_set res = root_.select_nodes(CADASTRAL_NUMBER_QUERY);
  return res;
}

pugi::xpath_node_set StandardParser::getEntitySpatialNodes(
    const pugi::xml_node& cadastralNumberNode) {
  pugi::xpath_node_set res;
  auto selector = localSelector("EntitySpatial");
  if (std::strcmp(cadastralNumberNode.name(), "CadastralBlock") == 0) {
    auto spatialNode = cadastralNumberNode.child("SpatialData");
    res = spatialNode.select_nodes(selector.c_str());
  } else {
    res = cadastralNumberNode.select_nodes(selector.c_str());
  }
  return res;
}

pugi::xpath_node_set StandardParser::getSpatialElementNodes(
    const pugi::xml_node& entitySpatialNode) {
  auto selector = localSelector("SpatialElement");
  return entitySpatialNode.select_nodes(selector.c_str());
}

pugi::xpath_node_set StandardParser::getOrdinateNodes(
    const pugi::xml_node& spatialElementNode) {
  auto selector = localSelector("Ordinate");
  return spatialElementNode.select_nodes(selector.c_str());
}

XMLSpatialInfo StandardParser::getSpatialInfo(
    const pugi::xml_node& cadastralNumberNode) {
  XMLSpatialInfo res(cadastralNumberNode.name(),
                     cadastralNumberNode.attribute("CadastralNumber").value());
  return res;
}

Point StandardParser::getXmlPoint(const pugi::xml_node& ordinateNode) {
  auto coords = getCoordinates(ordinateNode);
  auto radius = getRadius(ordinateNode);
  return Point(coords.first, coords.second, radius);
}

std::pair<double, double> StandardParser::getCoordinates(
    const pugi::xml_node& ordinateNode) {
  return {boost::lexical_cast<double>(ordinateNode.attribute("X").value()),
          boost::lexical_cast<double>(ordinateNode.attribute("Y").value())};
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
  BOOST_LOG_TRIVIAL(debug) << "Got " << res.size() << " CadastralItems";
  return res;
}

std::vector<std::vector<Point>> StandardParser::getEntitySpatialItems(
    const pugi::xml_node& entitySpatialNode) {
  std::vector<std::vector<Point>> res;
  for (auto& spatialElement : getSpatialElementNodes(entitySpatialNode)) {
    auto spatialElementItems = getSpatialElementItems(spatialElement.node());
    res.push_back(spatialElementItems);
  }
  BOOST_LOG_TRIVIAL(debug) << "Got " << res.size() << " EntitySpatialItems";
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
  BOOST_LOG_TRIVIAL(debug) << "Got " << res.size() << " SpatialElementItems";
  return res;
}

XMLParser::xmlSpatials_t StandardParser::getXMLSpatials() {
  xmlSpatials_t res;
  for (auto& cadastral : getCadastralNumberNodes()) {
    auto spatialInfo = getSpatialInfo(cadastral.node());
    BOOST_LOG_TRIVIAL(debug) << spatialInfo;
    XMLSpatial xmlSpatial(spatialInfo);
    auto cadastralItems = getCadastralItems(cadastral.node());
    BOOST_LOG_TRIVIAL(debug)
        << "Got " << cadastralItems.size() << " CadastralItems";
    xmlSpatial.spatial()->append(cadastralItems);
    res.push_back(xmlSpatial);
  }
  return res;
}

XMLInfo StandardParser::getXMLInfo() {}

}  // namespace rrt
