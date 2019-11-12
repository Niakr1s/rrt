#ifndef XMLSPATIALINFO_H
#define XMLSPATIALINFO_H

#include <iostream>
#include <string>

#include "cadastralnumber.h"

namespace rrt {

class XMLSpatialInfo {
 public:
  XMLSpatialInfo(std::string type, std::string cadastralNumber);

  friend std::ostream& operator<<(std::ostream& out, const XMLSpatialInfo& rhs);

  std::string type() const;
  std::string cadastralNumber() const;

 private:
  std::string type_;
  CadastralNumber cadastralNumber_;
};

std::ostream& operator<<(std::ostream& out, const XMLSpatialInfo& rhs);

}  // namespace rrt

#endif  // XMLSPATIALINFO_H
