#include "xmltreeview.h"

#include <QDebug>
#include <exception>
#include <future>
#include "xml.h"
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

void XMLTreeView::onNewXMLFiles(QVector<QFileInfo> xmlFiles) {
  for (auto& xmlFile : xmlFiles) {
    if (!xmlFile.exists()) {
      continue;
    }
    try {
      rrt::XML xml(xmlFile.absoluteFilePath().toStdWString());
      static_cast<XMLTreeModel*>(model())->appendSpatials(xml.xmlSpatials());
    } catch (std::exception& e) {
      qDebug() << e.what();
    }
  }
  expandAll();
}

void XMLTreeView::onDxfClose() {
  rootItem()->forEach([](XMLTreeItem* item) { item->turnOffIntersectsFlag(); });
}

XMLTreeItem* XMLTreeView::rootItem() {
  return static_cast<XMLTreeItem*>(model()->index(0, 0).internalPointer());
}
