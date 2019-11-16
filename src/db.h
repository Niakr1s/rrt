#ifndef DB_H
#define DB_H

#include <memory>
#include "idb.h"

namespace rrt {

class DB {
 public:
  static IDB* get();

  template <typename T>
  static void set() {
    iDB = std::make_unique<T>();
  }

 private:
  static std::unique_ptr<IDB> iDB;
};

}  // namespace rrt

#endif  // DB_H
