#include "xmlcadastralnumberinfo.h"

namespace rrt {

XMLCadastralNumberInfo::XMLCadastralNumberInfo(std::string type,
                                               std::string cadastralNumber)
    : type_(type), cadastralNumber_(cadastralNumber) {}

std::string XMLCadastralNumberInfo::type() const {
  return type_;
}

std::string XMLCadastralNumberInfo::cadastralNumber() const {
  return cadastralNumber_;
}

std::ostream& operator<<(std::ostream& out, const XMLCadastralNumberInfo& rhs) {
  return out << rhs.type_ << ": " << rhs.cadastralNumber_;
}

}  // namespace rrt
