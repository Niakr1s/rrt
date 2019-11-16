#ifndef IDB_H
#define IDB_H

#include <memory>
#include "cadastralnumber.h"
#include "xmlspatial.h"

namespace rrt {

class IDB {
 public:
  IDB();
  virtual ~IDB() = default;

  virtual void pushToDB(const XMLSpatial& xmlSpatial) = 0;
  virtual std::shared_ptr<XMLSpatial> getFromDB(
      const CadastralNumber& cadastralNumber) = 0;

  // table names
 protected:
  const char* XML_SPATIAL_INFO_TYPE = "XML_SPATIAL_INFO_TYPE";
  const char* XML_SPATIAL_INFO_CADASTRAL_NUMBER =
      "XML_SPATIAL_INFO_CADASTRAL_NUMBER";
  const char* XML_INFO_TYPE = "XML_INFO_TYPE";
  const char* XML_INFO_DATE = "XML_INFO_DATE";
  const char* XML_INFO_ORDER_NUMBER = "XML_INFO_ORDER_NUMBER";
  const char* XML_INFO_SPATIAL_INFO_TYPE = "XML_INFO_SPATIAL_INFO_TYPE";
  const char* XML_INFO_SPATIAL_INFO_CADASTRAL_NUMBER =
      "XML_INFO_SPATIAL_INFO_CADASTRAL_NUMBER";
};

}  // namespace rrt

#endif  // IDB_H
