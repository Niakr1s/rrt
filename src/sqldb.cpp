#include "sqldb.h"

#include <fmt/core.h>
#include <boost/filesystem.hpp>
#include <boost/log/trivial.hpp>
#include <functional>

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

void SqlDB::clearDB() {
  exec("DELETE FROM spatial");
}

void rrt::SqlDB::pushToDB(const rrt::XMLSpatial& xmlSpatial) {
  auto xss = xmlSpatial.serialize();
  exec(fmt::format(
      R"***(
         INSERT OR IGNORE INTO spatial (
                   spatial_type, spatial_cadastral_number,
                   root_type, root_cadastral_number,
                   xml_type, xml_date, xml_order_number,
                   rect, polygons, linestrings, circle_polygons, circles
      )
                   VALUES ("{}", "{}", "{}", "{}", "{}", "{}", "{}", "{}", "{}", "{}", "{}", "{}");
         )***",
      xss.spatial_type, xss.spatial_cadastral_number, xss.root_type,
      xss.root_cadastral_number, xss.xml_type, xss.xml_date,
      xss.xml_order_number, xss.rect, xss.polygons, xss.linestrings,
      xss.circle_polygons, xss.circles));
}

std::shared_ptr<rrt::XMLSpatial> rrt::SqlDB::getFromDB(
    const rrt::CadastralNumber& cadastralNumber,
    const std::string& date,
    const std::string& orderNumber) {
  std::vector<XMLSpatialSerialized> xmlSpatialSerialized;

  std::string orderQueue =
      orderNumber.empty()
          ? ""
          : fmt::format("AND xml_order_number = \"{}\"", orderNumber);

  std::string queue;
  if (date.empty()) {
    queue = fmt::format(R"***(
                        SELECT * FROM spatial
                        WHERE spatial_cadastral_number = "{}" {}
                        ORDER BY date(xml_date) DESC Limit 1;
                          )***",
                        cadastralNumber.string(), orderQueue);
  } else {
    queue = fmt::format(R"***(
                          SELECT * FROM spatial
                          WHERE spatial_cadastral_number = "{}" {} AND xml_date = "{}";
                            )***",
                        cadastralNumber.string(), orderQueue, date);
  }

  exec(queue, &makeXMLSpatialSerialized, &xmlSpatialSerialized);
  if (xmlSpatialSerialized.empty()) {
    throw std::runtime_error(
        fmt::format("SqlDB::getFromDB: {}, {}, {}: no such item",
                    cadastralNumber.string(), date, orderNumber));
  }
  auto res = std::make_shared<XMLSpatial>(xmlSpatialSerialized[0]);
  return res;
}

int SqlDB::makeXMLSpatialSerialized(void* xmlSpatialSerialized,
                                    int argc,
                                    char** argv,
                                    char** azColName) {
  auto spa =
      static_cast<std::vector<XMLSpatialSerialized>*>(xmlSpatialSerialized);

  for (auto i = 0; i < argc; i++) {
    if (std::strcmp(azColName[i], "id") == 0) {
      spa->push_back(XMLSpatialSerialized());
    }
    (*spa).back().insert(azColName[i], argv[i]);
  }
  return 0;
}

void rrt::SqlDB::init() {
  open();
  BOOST_LOG_TRIVIAL(info) << "SqlDB::initDB";

  XMLSpatialSerialized xmlSpatialInit;
  exec(
      R"***(
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

void SqlDB::exec(const std::string& cmd,
                 int (*cb)(void*, int, char**, char**) /*= &execCallback*/,
                 void* arg /*= nullptr*/) {
  int rc = sqlite3_exec(db_, cmd.c_str(), cb, arg, &zErrMsg);
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

IDB::xmlSpatials_t rrt::SqlDB::getAllLastFromDB() {
  std::vector<XMLSpatialSerialized> xmlSpatialSerialized;

  std::string queue;
  queue = (R"***(
           SELECT *, max(xml_date) FROM spatial
           GROUP BY spatial_cadastral_number;
                )***");
  exec(queue, &makeXMLSpatialSerialized, &xmlSpatialSerialized);

  IDB::xmlSpatials_t res;

  for (auto& xss : xmlSpatialSerialized) {
    auto spa = std::make_shared<XMLSpatial>(xss);
    res.push_back(spa);
  }

  return res;
}

}  // namespace rrt
