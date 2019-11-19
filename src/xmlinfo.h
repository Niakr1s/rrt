#ifndef XMLINFO_H
#define XMLINFO_H

#include <boost/date_time/gregorian/gregorian.hpp>
#include <string>
#include "cadastralnumber.h"
#include "xmlspatialinfo.h"

namespace rrt {

class XMLInfo {
 public:
  XMLInfo();
  XMLInfo(std::string type,
          std::string dateString,
          std::string orderNumber,
          const XMLSpatialInfo& rootSpatialInfo);

  std::string type() const;
  boost::gregorian::date date() const;
  std::string dateString() const;
  std::string orderNumber() const;
  const XMLSpatialInfo& rootSpatialInfo() const;

 private:
  std::string type_;
  boost::gregorian::date date_;
  std::string orderNumber_;
  XMLSpatialInfo rootSpatialInfo_;
};

}  // namespace rrt

#endif  // XMLINFO_H
