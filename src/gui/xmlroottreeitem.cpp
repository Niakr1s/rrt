#include "xmlroottreeitem.h"

#include "db.h"
#include "util.h"

XMLRootTreeItem::XMLRootTreeItem() {}

XMLTreeItem* XMLRootTreeItem::get() {
  return root_;
}

// copied from XMLTreeModel::appendSpatials
void XMLRootTreeItem::appendSpatials(const rrt::xmlSpatials_t& spatials,
                                     bool fromDB) {
  for (auto& spatial : spatials) {
    auto path = preparePath(spatial);

    XMLTreeItem* idx = root_;
    for (auto& str : path) {
      bool found = false;
      for (int i = 0; i != idx->childCount(); ++i) {
        auto child = idx->childs_[i];
        if (child->strID() == str) {
          idx = child;
          found = true;
          break;
        }
      }
      if (!found) {
        auto newChild = new XMLTreeItem();
        newChild->setData(0, QString::fromStdString(str));
        idx->childs_.push_back(newChild);
        idx = newChild;
      }
    }
    idx->appendSpatial(spatial, fromDB);
  }
  root_->dumpInfo();
}

XMLTreeItem* XMLRootTreeItem::root_ = new XMLTreeItem();
