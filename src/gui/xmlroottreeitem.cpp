#include "xmlroottreeitem.h"

XMLRootTreeItem::XMLRootTreeItem() {}

XMLTreeItem* XMLRootTreeItem::get() {
  return root_;
}

// copied from XMLTreeModel::appendSpatials
void XMLRootTreeItem::appendSpatial(const rrt::xmlSpatial_t& spatial,
                                    bool fromDB) {
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
      newChild->parent_ = idx;
      newChild->setData(0, QString::fromStdString(str));
      idx->childs_.push_back(newChild);
      idx = newChild;
    }
  }
  idx->appendSpatial(spatial, fromDB);
}

XMLTreeItem* XMLRootTreeItem::root_ = new XMLTreeItem();

std::vector<std::string> preparePath(rrt::xmlSpatial_t spatial) {
  auto path = spatial->xmlSpatialInfo().cadastralNumber().strings();
  while (path.size() != 3) {
    path.pop_back();
  }
  path.push_back(spatial->xmlSpatialInfo().type());
  path.push_back(spatial->xmlSpatialInfo().cadastralNumber().string());
  return path;
}
