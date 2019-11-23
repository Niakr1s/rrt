#ifndef XMLTREEVIEW_H
#define XMLTREEVIEW_H

#include <QFileInfo>
#include <QMenu>
#include <QModelIndex>
#include <QTreeView>
#include <QVector>
#include <memory>
#include <string>
#include "boost/filesystem/path.hpp"
#include "spatial.h"
#include "xmltreeitem.h"
#include "xmltreemodel.h"
#include "xmltreesortfilterproxymodel.h"

namespace bf = boost::filesystem;

class XMLTreeView : public QTreeView {
  Q_OBJECT

 public:
  XMLTreeView(QWidget* parent = nullptr);

  QVector<std::string> intersectsResult() const;

  void expandUntilRoot(QModelIndex item);
  void collapseAll();
  void expand(QModelIndex idx, int count = 0);

 signals:
  void startProcessingXMLsSignal(int size);
  void oneXMLProcessedSignal(int pos, int max);
  void XMLtoDBStartSignal();
  void XMLtoDBEndSignal();
  void endProcessingXMLsSignal(QVector<QString> err);
  void endProcessingDXFSignal(int found);

 public slots:
  void onNewDXFSpatial(std::shared_ptr<rrt::Spatial> spatial);
  void onNewXMLFiles(QVector<QFileInfo> xmlFiles);
  void onDxfClose();
  void onRowsInserted(QModelIndex sourceParent, int first, int last);
  void onCopySemicolonButtonClick();
  void onCopyNewlineButtonClick();
  void onCustomContextMenuRequested(QPoint p);
  void onExportAction();
  void onExpandButtonToggled(bool expand);

 private:
  bf::path cwd_;
  bf::path dataPath_;
  QMenu* exportMenu_;
  XMLTreeSortFilterProxyModel* model_;

 private:
  XMLTreeModel* xmlModel();
  void initDirectories() const;
  QVector<std::string> intersectsResult_;
  std::shared_ptr<rrt::Spatial> spatial_;
};

#endif  // XMLTREEVIEW_H
