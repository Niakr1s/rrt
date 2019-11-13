#include "xmlinfo.h"

#include <boost/algorithm/string.hpp>

namespace rrt {

XMLInfo::XMLInfo() {}

XMLInfo::XMLInfo(std::string type, std::string date, std::string number)
    : type_(type),
      date_(boost::gregorian::from_string(date)),
      orderNumber_(number) {}

std::string XMLInfo::type() const {
  return type_;
}

std::string XMLInfo::orderNumber() const {
  return orderNumber_;
}

std::string XMLInfo::date() const {
  return boost::gregorian::to_iso_extended_string(date_);
}

}  // namespace rrt
