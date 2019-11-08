#include "xml.h"

#include <fmt/core.h>
#include <boost/log/trivial.hpp>
#include <stdexcept>
#include <string>
#include "common.h"

namespace rrt {

XML::XML(const char* path) : path_(path) {
  pugi::xml_document root;
  auto res = root.load_file(path_.c_str());
  if (res.status != pugi::status_ok) {
    throw(std::invalid_argument(
        fmt::format("{}: {}", res.description(), path_.string())));
  }
  BOOST_LOG_TRIVIAL(info) << "Succesfully opened: " << path_;
  parser_ = XMLParser::chooseParser(root);
  if (parser_ == nullptr) {
    throw(std::runtime_error(
        fmt::format("Couldn't choose parser for {}", path_.string())));
  }
  cadastralNumbers_ = parser_->getCadastralNumbers();
}

XML::cadastralNumbers_t XML::cadastralNumbers() const {
  return cadastralNumbers_;
}

}  // namespace rrt
