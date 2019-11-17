#include <algorithm>
#include <iostream>
#include <stdexcept>
#include "db.h"
#include "sqldb.h"
#include "xml.h"

int main(int argc, char* argv[]) {
  rrt::DB::set<rrt::SqlDB>();
  auto xml = rrt::XML("KPT CadastralBlock 77 03 0009007.xml");
  for (auto& spatial : xml.xmlSpatials()) {
    rrt::DB::get()->pushToDB(*spatial);
  }
  auto got =
      rrt::DB::get()->getFromDB(rrt::CadastralNumber("77:03:0009007:1225"));
  return 0;
}
