#ifndef XMLINFO_H
#define XMLINFO_H

#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/filesystem/path.hpp>
#include <string>
#include "cadastralnumber.h"
#include "xmlspatialinfo.h"

namespace bf = boost::filesystem;

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
  bf::path path() const;
  void setPath(const bf::path& path);

 private:
  std::string type_;
  boost::gregorian::date date_;
  std::string orderNumber_;
  XMLSpatialInfo rootSpatialInfo_;
  bf::path path_;
};

}  // namespace rrt

#endif  // XMLINFO_H
