#include "xml.h"

#include <fmt/core.h>
#include <boost/log/trivial.hpp>
#include <pugixml.hpp>
#include <stdexcept>
#include <string>

namespace rrt {

XML::XML(const std::string& path) : path_(path) {
  pugi::xml_document root;
  if (auto res = root.load_file(path_.c_str()); res.status != pugi::status_ok) {
    throw(std::invalid_argument(fmt::format(
        "XML: wrong input file: {}. {}", path_.string(), res.description())));
  }
  BOOST_LOG_TRIVIAL(info) << "XML: succesfully opened: " << path_;
  parser_ = XMLParser::chooseParser(root);
  if (parser_ == nullptr) {
    throw(std::runtime_error(
        fmt::format("XML: couldn't choose parser for {}", path_.string())));
  }
  spatials_ = parser_->getXMLSpatials();
  BOOST_LOG_TRIVIAL(info) << "XML: succesfully parsed: " << path_;
}

const XML::xmlSpatials_t& XML::xmlSpatials() const {
  return spatials_;
}

XML::xmlSpatials_t& XML::xmlSpatials() {
  return spatials_;
}

}  // namespace rrt
