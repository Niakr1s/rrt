#ifndef XMLTREEVIEW_H
#define XMLTREEVIEW_H

#include <QFileInfo>
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

 signals:
  void errXMLsSignal(QVector<QString>);

 public slots:
  void onNewDXFSpatial(std::shared_ptr<rrt::Spatial> spatial);
  void onNewXMLFiles(QVector<QFileInfo> xmlFiles);
  void onDxfClose();

 private:
  bf::path cwd_;
  bf::path dataPath_;

 private:
  XMLTreeModel* xmlModel();
  void initDirectories() const;
};

#endif  // XMLTREEVIEW_H
