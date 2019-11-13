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
          std::string date,
          std::string orderNumber,
          const XMLSpatialInfo& spatialInfo);

  std::string type() const;
  std::string date() const;
  std::string orderNumber() const;
  const CadastralNumber& cadastralNumber() const;

 private:
  std::string type_;
  boost::gregorian::date date_;
  std::string orderNumber_;
  XMLSpatialInfo spatialInfo_;
};

}  // namespace rrt

#endif  // XMLINFO_H
