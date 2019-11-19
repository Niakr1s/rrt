#include "xml.h"

#include <fmt/core.h>
#include <boost/filesystem.hpp>
#include <boost/log/trivial.hpp>
#include <pugixml.hpp>
#include <stdexcept>
#include <string>

namespace rrt {

XML::XML(const std::string& path)
    : XML(std::wstring(path.cbegin(), path.cend())) {}

XML::XML(const std::wstring& path) : path_(path) {
  pugi::xml_document root;
  BOOST_LOG_TRIVIAL(info) << "XML: trying to open: " << path_;
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
  if (spatials_.empty()) {
    throw(std::runtime_error(
        fmt::format("XML: empty spatials after parsing {}", path_.string())));
  }
  xmlInfo_ = std::make_shared<XMLInfo>(parser_->getXMLInfo());
  addXmlInfoToSpatials();
  BOOST_LOG_TRIVIAL(info) << "XML: succesfully parsed: " << path_;
}

const XML::xmlSpatials_t& XML::xmlSpatials() const {
  return spatials_;
}

XML::xmlSpatials_t& XML::xmlSpatials() {
  return spatials_;
}

void XML::saveToDXF(std::string path /*= ""*/,
                    DXF::Version version /* = DXF::Version::AC1015*/) {
  if (path.empty()) {
    auto exportPath(path_);
    exportPath.replace_extension("dxf");
    path = exportPath.string();
  }

  DXF dxf;
  for (auto& xmlSpatial : spatials_) {
    dxf.drawSpatial(xmlSpatial->xmlSpatialInfo().cadastralNumber().string(),
                    xmlSpatial->xmlSpatialInfo().type(), xmlSpatial->spatial(),
                    xmlSpatial->color());
  }
  dxf.fileExport(path, version);
}

void XML::renameFile() {
  std::string newFilenameStr = fmt::format(
      "{} {} {}{}", xmlInfo_->type(),
      xmlInfo_->rootSpatialInfo().cadastralNumber().underscoredString(),
      xmlInfo_->dateString(), path_.extension().string());
  bf::path newFilename(newFilenameStr);
  bf::path newPath = path_.parent_path();
  newPath.append(newFilenameStr);
  bf::rename(path_, newPath);
  path_ = newPath;
}

const XMLInfo& XML::xmlInfo() const {
  return *xmlInfo_;
}

XML::xmlSpatials_t XML::intersects(const Spatial& spatial) const {
  xmlSpatials_t res;
  for (auto& it : spatials_) {
    if (it->spatial()->intersects(spatial)) {
      res.push_back(it);
    }
  }
  return res;
}

void XML::addXmlInfoToSpatials() {
  for (auto& spatial : spatials_) {
    spatial->add(xmlInfo_);
  }
}

}  // namespace rrt
