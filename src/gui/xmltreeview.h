#ifndef XMLTREEVIEW_H
#define XMLTREEVIEW_H

#include <QFileInfo>
#include <QHash>
#include <QMenu>
#include <QMessageBox>
#include <QModelIndex>
#include <QStringList>
#include <QTreeView>
#include <QVector>
#include <memory>
#include <string>
#include "spatial.h"
#include "typedefs.h"
#include "xml.h"
#include "xmlspatial.h"
#include "xmltreeitem.h"
#include "xmltreemodel.h"
#include "xmltreesortfilterproxymodel.h"

class XMLTreeView : public QTreeView {
  Q_OBJECT

 public:
  XMLTreeView(QWidget* parent = nullptr);

  void expandUntilRoot(QModelIndex item);
  void expand(QModelIndex idx, int count = 0);

  XMLTreeModel* xmlModel();

 signals:
  void exportToDXF(QModelIndex sourceIndex, QString fileName);

 public slots:
  void collapseAll();
  void showIntersected(std::shared_ptr<DXFResult>);
  void disableIntersectionsFiltering();
  void expandChildsUntilRoot(QModelIndex sourceParent, int first, int last);
  void showCustomContextMenu(QPoint p);
  void expandIf(bool expand);
  void exportSelectedToDXF();
  void showErrXMLsMessageBox(QStringList errXMLPaths);
  void showMessageBox(QString title, QString msg, QMessageBox::Icon icon);

 private:
  QMenu* rightClickMenu_;
  QAction* exportAction_;
  XMLTreeModel* model_;
  XMLTreeSortFilterProxyModel* proxyModel_;

 private:
  void initModel();
  void initRightClickMenu();
  void connectAll();
};

#endif  // XMLTREEVIEW_H
