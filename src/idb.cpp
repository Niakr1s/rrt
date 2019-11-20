#include "idb.h"

namespace rrt {

IDB::IDB() = default;

void IDB::pushToDB(const XML& xml) {
  for (auto& spatial : xml.xmlSpatials()) {
    pushToDB(*spatial);
  }
}

}  // namespace rrt
