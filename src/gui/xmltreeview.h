#ifndef XMLTREEVIEW_H
#define XMLTREEVIEW_H

#include <QFileInfo>
#include <QHash>
#include <QMenu>
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
  void expandUntilRootNew(QModelIndex item = QModelIndex());
  void collapseAll();
  void expand(QModelIndex idx, int count = 0);

  XMLTreeModel* xmlModel();

 signals:
  void startProcessingXMLsSignal(int size);
  void newXMLSpatialsSignal(rrt::xmlSpatials_t, bool fromDB);
  void oneXMLProcessedSignal(int pos, int max);

 public slots:
  void onEndAppendingXMLs();
  void showIntersected(std::shared_ptr<DXFResult>);
  void disableIntersectionsFiltering();
  void onRowsInserted(QModelIndex sourceParent, int first, int last);
  void onCustomContextMenuRequested(QPoint p);
  void onExportAction();
  void onExpandButtonToggled(bool expand);
  void onNewXMLSpatials(rrt::xmlSpatials_t, bool fromDB);

 private:
  QMenu* rightClickMenu_;
  QAction* exportAction_;
  XMLTreeModel* model_;
  XMLTreeSortFilterProxyModel* proxyModel_;
  std::shared_ptr<rrt::Spatial> spatial_;

 private:
  void initModel();
  void initRightClickMenu();
  void connectAll();
};

#endif  // XMLTREEVIEW_H
