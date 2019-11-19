#include "xmlinfo.h"

#include <boost/algorithm/string.hpp>

namespace rrt {

XMLInfo::XMLInfo() {}

XMLInfo::XMLInfo(std::string type,
                 std::string date,
                 std::string number,
                 const XMLSpatialInfo& spatialInfo)
    : type_(type),
      date_(boost::gregorian::from_string(date)),
      orderNumber_(number),
      rootSpatialInfo_(spatialInfo) {}

std::string XMLInfo::type() const {
  return type_;
}

boost::gregorian::date XMLInfo::date() const {
  return date_;
}

std::string XMLInfo::orderNumber() const {
  return orderNumber_;
}

const XMLSpatialInfo& XMLInfo::rootSpatialInfo() const {
  return rootSpatialInfo_;
}

std::string XMLInfo::dateString() const {
  return boost::gregorian::to_iso_extended_string(date_);
}

}  // namespace rrt
