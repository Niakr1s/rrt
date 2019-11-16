#include "db.h"

namespace rrt {

IDB* DB::get() {
  return iDB.get();
}

std::unique_ptr<IDB> DB::iDB = nullptr;  // TODO

}  // namespace rrt
