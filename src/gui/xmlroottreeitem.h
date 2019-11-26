#ifndef XMLROOTTREEITEM_H
#define XMLROOTTREEITEM_H

#include "xmlspatial.h"
#include "xmltreeitem.h"

class XMLRootTreeItem {
 public:
  XMLRootTreeItem();

  static XMLTreeItem* get();

  static void appendSpatial(const rrt::xmlSpatial_t& spatials,
                            bool fromDB = false);

 private:
  static XMLTreeItem* root_;
};

std::vector<std::string> preparePath(rrt::xmlSpatial_t spatial);

#endif  // XMLROOTTREEITEM_H
