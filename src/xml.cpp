#include "xml.h"

#include <fmt/core.h>
#include <boost/filesystem.hpp>
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
    dxf.drawSpatial(xmlSpatial->info().cadastralNumber().string(),
                    xmlSpatial->info().type(), xmlSpatial->spatial(),
                    xmlSpatial->color());
  }
  dxf.fileExport(path, version);
}

void XML::renameFile() {
  std::string newFilenameStr =
      fmt::format("{} {} {}{}", xmlInfo_->type(),
                  xmlInfo_->spatialInfo().cadastralNumber().underscoredString(),
                  xmlInfo_->date(), path_.extension().string());
  bf::path newFilename(newFilenameStr);
  bf::path newPath = path_.parent_path();
  newPath.append(newFilenameStr);
  bf::rename(path_, newPath);
  path_ = newPath;
}

const XMLInfo& XML::xmlInfo() const {
  return *xmlInfo_;
}

void XML::addXmlInfoToSpatials() {
  for (auto& spatial : spatials_) {
    spatial->add(xmlInfo_);
  }
}

}  // namespace rrt
