#include "xmlspatialinfo.h"

namespace rrt {

XMLSpatialInfo::XMLSpatialInfo() {}

XMLSpatialInfo::XMLSpatialInfo(std::string type, std::string cadastralNumber)
    : type_(type), cadastralNumber_(cadastralNumber) {}

std::string XMLSpatialInfo::type() const {
  return type_;
}

const CadastralNumber& XMLSpatialInfo::cadastralNumber() const {
  return cadastralNumber_;
}

std::ostream& operator<<(std::ostream& out, const XMLSpatialInfo& rhs) {
  return out << rhs.type_ << ": " << rhs.cadastralNumber_;
}

}  // namespace rrt
