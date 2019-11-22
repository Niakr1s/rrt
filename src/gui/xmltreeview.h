#ifndef XMLTREEVIEW_H
#define XMLTREEVIEW_H

#include <QFileInfo>
#include <QMenu>
#include <QTreeView>
#include <QVector>
#include <memory>
#include "boost/filesystem/path.hpp"
#include "spatial.h"
#include "xmltreeitem.h"
#include "xmltreemodel.h"

namespace bf = boost::filesystem;

class XMLTreeView : public QTreeView {
  Q_OBJECT

 public:
  XMLTreeView(QWidget* parent = nullptr);

  QVector<std::string> intersectsResult() const;

 signals:
  void startProcessingXMLsSignal(int size);
  void oneXMLProcessedSignal(int pos, int max);
  void endProcessingXMLsSignal(QVector<QString> err);

 public slots:
  void onNewDXFSpatial(std::shared_ptr<rrt::Spatial> spatial);
  void onNewXMLFiles(QVector<QFileInfo> xmlFiles);
  void onDxfClose();
  void onRowsInserted(const QModelIndex& parent, int first, int last);
  void onCopySemicolonButtonClick();
  void onCopyNewlineButtonClick();
  void onCustomContextMenuRequested(QPoint p);
  void onExportAction();

 private:
  bf::path cwd_;
  bf::path dataPath_;
  QMenu* exportMenu_;

 private:
  XMLTreeModel* xmlModel();
  void initDirectories() const;
  QVector<std::string> intersectsResult_;
  std::shared_ptr<rrt::Spatial> spatial_;
};

#endif  // XMLTREEVIEW_H
