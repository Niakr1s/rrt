#ifndef SQLDB_H
#define SQLDB_H

#include "idb.h"

namespace rrt {

class SqlDB : public IDB {
 public:
  SqlDB();

 public:
  void pushToDB(const XMLSpatial& xmlSpatial) override;

  std::shared_ptr<XMLSpatial> getFromDB(
      const CadastralNumber& cadastralNumber) override;
};

}  // namespace rrt

#endif  // SQLDB_H
