#include "xmlinfo.h"

#include <boost/algorithm/string.hpp>

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

std::string XMLInfo::fsSafeNumber() const {
  std::string res(number_);
  boost::replace_all(res, ":", "_");
  return res;
}

std::string XMLInfo::date() const {
  return boost::gregorian::to_iso_extended_string(date_);
}

}  // namespace rrt
