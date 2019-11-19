#ifndef XMLTREEVIEW_H
#define XMLTREEVIEW_H

#include <QFileInfo>
#include <QTreeView>
#include <memory>
#include "spatial.h"
#include "xmltreeitem.h"

class XMLTreeView : public QTreeView {
  Q_OBJECT

 public:
  XMLTreeView(QWidget* parent = nullptr);

 public slots:
  void onNewDXFSpatial(std::shared_ptr<rrt::Spatial> spatial);
  void onDxfClose();

 private:
  XMLTreeItem* rootItem();
};

#endif  // XMLTREEVIEW_H
