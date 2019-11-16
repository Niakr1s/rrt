#include "sqldb.h"

namespace rrt {

SqlDB::SqlDB() {}

}  // namespace rrt

void rrt::SqlDB::pushToDB(const rrt::XMLSpatial& xmlSpatial) {}

std::shared_ptr<rrt::XMLSpatial> rrt::SqlDB::getFromDB(
    const rrt::CadastralNumber& cadastralNumber) {}
