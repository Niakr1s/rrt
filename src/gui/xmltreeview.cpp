#include "xmltreeview.h"

#include <future>
#include "xmltreedelegate.h"
#include "xmltreemodel.h"

XMLTreeView::XMLTreeView(QWidget* parent) : QTreeView(parent) {
  XMLTreeModel* model = new XMLTreeModel();

  setModel(model);
  setSortingEnabled(true);
  setEditTriggers(QTreeView::NoEditTriggers);
  setMinimumHeight(400);

  auto delegate = new XMLTreeDelegate();
  setItemDelegate(delegate);

  expandAll();
}

void XMLTreeView::onNewDXFSpatial(std::shared_ptr<rrt::Spatial> spatial) {
  rootItem()->forEach([&](XMLTreeItem* item) {
    std::async(&XMLTreeItem::intersects, item, *spatial);
  });
}

void XMLTreeView::onDxfClose() {
  rootItem()->forEach([](XMLTreeItem* item) { item->turnOffIntersectsFlag(); });
}

XMLTreeItem* XMLTreeView::rootItem() {
  return static_cast<XMLTreeItem*>(model()->index(0, 0).internalPointer());
}
