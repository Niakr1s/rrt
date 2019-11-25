#ifndef XMLROOTTREEITEM_H
#define XMLROOTTREEITEM_H

#include "xmlspatial.h"
#include "xmltreeitem.h"

class XMLRootTreeItem {
 public:
  XMLRootTreeItem();

  static XMLTreeItem* get();

  static void appendSpatials(const rrt::xmlSpatials_t& spatials,
                             bool fromDB = false);

 private:
  static XMLTreeItem* root_;
};

#endif  // XMLROOTTREEITEM_H
