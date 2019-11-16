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
};

}  // namespace rrt

#endif  // IDB_H
