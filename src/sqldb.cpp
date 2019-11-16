#include "sqldb.h"

#include <fmt/core.h>
#include <boost/filesystem.hpp>
#include <boost/log/trivial.hpp>

namespace bf = boost::filesystem;

namespace rrt {

SqlDB::SqlDB() : IDB() {
  if (!bf::exists(DB_FILE_PATH)) {
    init();
  } else {
    open();
  }
}

SqlDB::~SqlDB() {
  sqlite3_close(db_);
}

void rrt::SqlDB::pushToDB(const rrt::XMLSpatial& xmlSpatial) {
  exec(fmt::format(
      R"***(
         INSERT OR IGNORE INTO spatial (
                   spatial_type, spatial_cadastral_number,
                   root_type, root_cadastral_number,
                   xml_type, xml_date, xml_order_number)
                   VALUES ("{}", "{}", "{}", "{}", "{}", "{}", "{}");
         )***",
      xmlSpatial.xmlSpatialInfo().type(),
      xmlSpatial.xmlSpatialInfo().cadastralNumber().string(),
      xmlSpatial.xmlInfo().rootSpatialInfo().type(),
      xmlSpatial.xmlInfo().rootSpatialInfo().cadastralNumber().string(),
      xmlSpatial.xmlInfo().type(), xmlSpatial.xmlInfo().date(),
      xmlSpatial.xmlInfo().orderNumber()));

  BOOST_LOG_TRIVIAL(info) << "SqlDB::pushToDB: Succesfully pushed";
}

std::shared_ptr<rrt::XMLSpatial> rrt::SqlDB::getFromDB(
    const rrt::CadastralNumber& cadastralNumber) {}

void rrt::SqlDB::init() {
  open();
  BOOST_LOG_TRIVIAL(info) << "SqlDB::initDB";

  exec(R"***(
       CREATE TABLE "spatial" (
       "id"	INTEGER,

       "spatial_type"	TEXT,
       "spatial_cadastral_number"	TEXT,

       "root_type"	TEXT,
       "root_cadastral_number"	TEXT,

       "xml_type"	TEXT,
       "xml_date"	TEXT,
       "xml_order_number"	TEXT,

       "rect"	TEXT,
       "polygons"	TEXT,
       "linestrings"	TEXT,
       "circle_polygons"	TEXT,
       "circles"	TEXT,

       PRIMARY KEY("id"),
       UNIQUE(spatial_cadastral_number, xml_order_number)
       );
       )***");
}

void SqlDB::open() {
  int rc;
  rc = sqlite3_open(DB_FILE_PATH.string().c_str(), &db_);
  if (rc) {
    throw(std::runtime_error(
        fmt::format("SqlDB::open: can't open database: {}", sqlite3_errmsg(db_))
            .c_str()));
  }
}

void SqlDB::exec(const std::string& cmd) {
  int rc = sqlite3_exec(db_, cmd.c_str(), &execCallback, nullptr, &zErrMsg);
  if (rc != SQLITE_OK) {
    BOOST_LOG_TRIVIAL(error) << "SqlDB::exec: SQL Error: " << zErrMsg;
    sqlite3_free(zErrMsg);
  }
}

int SqlDB::execCallback(void* NotUsed,
                        int argc,
                        char** argv,
                        char** azColName) {
  int i;
  for (i = 0; i < argc; i++) {
    BOOST_LOG_TRIVIAL(debug)
        << fmt::format("{} = {}", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  return 0;
}

}  // namespace rrt
