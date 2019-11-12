#include "xmlinfo.h"

namespace rrt {

XMLInfo::XMLInfo() {}

XMLInfo::XMLInfo(std::string type, std::string date, std::string number)
    : type_(type),
      date_(boost::gregorian::from_string(date)),
      number_(number) {}

std::string XMLInfo::type() const {
  return type_;
}

std::string XMLInfo::number() const {
  return number_;
}

boost::gregorian::date XMLInfo::date() const {
  return date_;
}

}  // namespace rrt
