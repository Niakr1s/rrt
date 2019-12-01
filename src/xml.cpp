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

XML::XML(const std::wstring& path) {
  bf::path p(path);
  pugi::xml_document root;
  BOOST_LOG_TRIVIAL(debug) << "XML: trying to open: " << p;
  if (auto res = root.load_file(p.c_str()); res.status != pugi::status_ok) {
    throw(std::invalid_argument(fmt::format("XML: wrong input file: {}. {}",
                                            p.string(), res.description())));
  }
  BOOST_LOG_TRIVIAL(debug) << "XML: succesfully opened: " << p;
  parser_ = XMLParser::chooseParser(root);
  if (parser_ == nullptr) {
    throw(std::runtime_error(
        fmt::format("XML: couldn't choose parser for {}", p.string())));
  }
  spatials_ = parser_->getXMLSpatials();
  if (spatials_.empty()) {
    throw(std::runtime_error(
        fmt::format("XML: empty spatials after parsing {}", p.string())));
  }
  xmlInfo_ = std::make_shared<XMLInfo>(parser_->getXMLInfo());
  xmlInfo_->setPath(p);
  addXmlInfoToSpatials();
  BOOST_LOG_TRIVIAL(debug) << "XML: succesfully parsed: " << p;
}

const xmlSpatials_t& XML::xmlSpatials() const {
  return spatials_;
}

xmlSpatials_t& XML::xmlSpatials() {
  return spatials_;
}

void XML::saveToDXF(std::string path /*= ""*/,
                    DXF::Version version /* = DXF::Version::AC1015*/) {
  if (path.empty()) {
    auto exportPath(this->path());
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

bf::path XML::renameFile() {
  auto p = path();
  std::string newFilenameStr =
      fmt::format("{} {} {}{}", xmlInfo_->type(),
                  xmlInfo_->rootSpatialInfo().cadastralNumber().safeString(),
                  xmlInfo_->dateString(), p.extension().string());
  bf::path newFilename(newFilenameStr);
  bf::path newPath = p.parent_path();
  newPath.append(newFilenameStr);
  bf::rename(p, newPath);
  p = newPath;
  xmlInfo_->setPath(p);
  return p;
}

void XML::copyFile(bf::path newPath) {
  if (!bf::exists(newPath)) {
    bf::copy(path(), newPath);
  }
  xmlInfo_->setPath(newPath);
}

const XMLInfo& XML::xmlInfo() const {
  return *xmlInfo_;
}

xmlSpatials_t XML::intersects(const Spatial& spatial) const {
  xmlSpatials_t res;
  for (auto& it : spatials_) {
    if (it->spatial()->intersects(spatial)) {
      res.push_back(it);
    }
  }
  return res;
}

bf::path XML::path() const {
  return xmlInfo_->path();
}

void XML::addXmlInfoToSpatials() {
  for (auto& spatial : spatials_) {
    spatial->add(xmlInfo_);
  }
}

}  // namespace rrt
