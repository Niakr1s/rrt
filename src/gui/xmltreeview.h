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
#include "boost/filesystem/path.hpp"
#include "spatial.h"
#include "typedefs.h"
#include "xml.h"
#include "xmltreeitem.h"
#include "xmltreemodel.h"
#include "xmltreesortfilterproxymodel.h"

namespace bf = boost::filesystem;

class XMLTreeView : public QTreeView {
  Q_OBJECT

 public:
  XMLTreeView(QWidget* parent = nullptr);

  void expandUntilRoot(QModelIndex item);
  void collapseAll();
  void expand(QModelIndex idx, int count = 0);

 signals:
  void startProcessingXMLsSignal(int size);
  void newXMLSpatialsSignal(rrt::XML::xmlSpatials_t, bool fromDB);
  void oneXMLProcessedSignal(int pos, int max);
  void XMLtoDBStartSignal();
  void XMLtoDBEndSignal();
  void endProcessingXMLsSignal(QStringList err);
  void endProcessingDXFSignal(std::shared_ptr<DXFResult>);

 public slots:
  void onNewDXFSpatial(std::shared_ptr<rrt::Spatial> spatial);
  void onNewXMLFiles(QVector<QFileInfo> xmlFiles);
  void onEndProcessingDXF(std::shared_ptr<DXFResult>);
  void onDxfClose();
  void onRowsInserted(QModelIndex sourceParent, int first, int last);
  void onCustomContextMenuRequested(QPoint p);
  void onExportAction();
  void onExpandButtonToggled(bool expand);
  void onNewXMLSpatials(rrt::XML::xmlSpatials_t, bool fromDB);

 private:
  bf::path cwd_;
  bf::path dataPath_;
  QMenu* rightClickMenu_;
  QAction* exportAction_;
  XMLTreeModel* model_;
  XMLTreeSortFilterProxyModel* proxyModel_;
  std::shared_ptr<rrt::Spatial> spatial_;

 private:
  XMLTreeModel* xmlModel();
  void loadDBSpatials();
  void initDirectories() const;
  void initModel();
  void initRightClickMenu();
  void connectAll();
};

#endif  // XMLTREEVIEW_H
