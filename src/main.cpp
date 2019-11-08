#include <algorithm>
#include <iostream>
#include <stdexcept>

#include <boost/lexical_cast.hpp>
#include <boost/log/trivial.hpp>
#include <pugixml.hpp>

#include "xml.h"

int main(int argc, char* argv[]) {
  rrt::XML xml("../../tests_data/KPT CadastralBlock 77 03 0009007.xml");

  return 0;
}
