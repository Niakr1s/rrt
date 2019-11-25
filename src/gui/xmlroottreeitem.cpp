#include "xmlroottreeitem.h"

XMLRootTreeItem::XMLRootTreeItem() {}

XMLTreeItem* XMLRootTreeItem::get() {
  return root_;
}

void XMLRootTreeItem::appendSpatials(const rrt::xmlSpatials_t& spatials,
                                     bool fromDB) {}

XMLTreeItem* XMLRootTreeItem::root_;
