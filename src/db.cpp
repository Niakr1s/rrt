#include "db.h"

#include "sqldb.h"

namespace rrt {

IDB* DB::get() {
  return iDB.get();
}

std::unique_ptr<IDB> DB::iDB = std::make_unique<SqlDB>();

}  // namespace rrt
