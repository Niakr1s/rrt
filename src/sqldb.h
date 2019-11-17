#ifndef SQLDB_H
#define SQLDB_H

#include <sqlite3.h>
#include <boost/filesystem/path.hpp>
#include "idb.h"

namespace bf = boost::filesystem;

namespace rrt {

class SqlDB : public IDB {
 public:
  SqlDB();
  ~SqlDB();

 public:
  void pushToDB(const XMLSpatial& xmlSpatial) override;

  std::shared_ptr<XMLSpatial> getFromDB(
      const CadastralNumber& cadastralNumber) override;

  static int makeXMLSpatialSerialized(void* xmlSpatialSerialized,
                                      int argc,
                                      char** argv,
                                      char** azColName);

  XMLSpatial::xmlSpatials_t getAllFromDB() override;

  void init();
  void open();
  void exec(const std::string& cmd,
            int (*cb)(void*, int, char**, char**) = &execCallback,
            void* arg = nullptr);
  static int execCallback(void* NotUsed,
                          int argc,
                          char** argv,
                          char** azColName);

 private:
  sqlite3* db_;
  char* zErrMsg = nullptr;
  const bf::path DB_FILE_PATH = "main.db";
};

}  // namespace rrt

#endif  // SQLDB_H
