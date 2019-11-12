#include "xmlspatialinfo.h"

namespace rrt {

XMLSpatialInfo::XMLSpatialInfo(std::string type, std::string cadastralNumber)
    : type_(type), cadastralNumber_(cadastralNumber) {}

std::string XMLSpatialInfo::type() const {
  return type_;
}

std::string XMLSpatialInfo::cadastralNumber() const {
  return cadastralNumber_.string();
}

std::ostream& operator<<(std::ostream& out, const XMLSpatialInfo& rhs) {
  return out << rhs.type_ << ": " << rhs.cadastralNumber_;
}

}  // namespace rrt
