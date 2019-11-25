#ifndef IDB_H
#define IDB_H

#include <memory>
#include <vector>
#include "cadastralnumber.h"
#include "xml.h"
#include "xmlspatial.h"

namespace rrt {

class IDB {
 public:
  using xmlSpatials_t = XMLSpatial::xmlSpatials_t;

  IDB();
  virtual ~IDB() = default;

  virtual void clearDB() = 0;

  virtual void pushToDB(const XMLSpatial& xmlSpatial) = 0;
  virtual void pushToDB(const XML& xml);

  virtual std::shared_ptr<XMLSpatial> getFromDB(
      const CadastralNumber& cadastralNumber,
      const std::string& date = "",
      const std::string& orderNumber = "") = 0;

  virtual xmlSpatials_t getAllLastFromDB() = 0;
};

}  // namespace rrt

#endif  // IDB_H
