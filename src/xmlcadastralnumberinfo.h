#ifndef XMLCADASTRALNUMBERINFO_H
#define XMLCADASTRALNUMBERINFO_H

#include <iostream>
#include <string>

namespace rrt {

class XMLCadastralNumberInfo {
 public:
  XMLCadastralNumberInfo(std::string type, std::string cadastralNumber);

  friend std::ostream& operator<<(std::ostream& out, const XMLCadastralNumberInfo& rhs);

  std::string type() const;
  std::string cadastralNumber() const;

 private:
  std::string type_;
  std::string cadastralNumber_;
};

std::ostream& operator<<(std::ostream& out, const XMLCadastralNumberInfo& rhs);

}  // namespace rrt

#endif  // XMLCADASTRALNUMBERINFO_H
