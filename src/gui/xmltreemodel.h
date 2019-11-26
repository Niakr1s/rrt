#ifndef XMLTREEMODEL_H
#define XMLTREEMODEL_H

#include <QAbstractItemModel>
#include <QFileInfo>
#include <QModelIndex>
#include <QObject>
#include <QVariant>
#include <QVector>
#include <boost/filesystem/path.hpp>
#include "xmlroottreeitem.h"
#include "xmlspatial.h"
#include "xmltreeitem.h"

namespace bf = boost::filesystem;

namespace {
const char* DATA_PATH = "data";
}

class XMLTreeModel : public QAbstractItemModel {
  Q_OBJECT
  friend class XMLTreeView;

 public:
  XMLTreeModel(QObject* parent = nullptr);

  void appendSpatialsSlow(const rrt::xmlSpatials_t& spatials,
                          bool fromDB = false);
  void appendSpatials(const rrt::xmlSpatials_t& spatials, bool fromDB = false);

  int size() const;

  XMLTreeItem* getItem(const QModelIndex& index) const;
  XMLTreeItem* getRootItem() const;

  void forEach(std::function<void(XMLTreeItem*)> fn);
  void forEach(QModelIndex idx, std::function<void(XMLTreeItem*)> fn);

 signals:
  void appendSpatialsFinishedSignal();
  void newXMLSpatialsSignal(rrt::xmlSpatials_t spatials, bool fromDB);
  void startProcessingSignal(int size);
  void oneProcessedSignal(int pos, int max);
  void endProcessingSignal();
  void DBBeginSignal();
  void DBEndSignal();
  void endProcessingXMLsSignal(QStringList err);

 public slots:
  void onXmlTreeItemDataChanged(XMLTreeItem* item);
  void appendXMLs(QVector<QFileInfo> xmlFiles);
  void onNewXMLSpatials(rrt::xmlSpatials_t spatials, bool fromDB);
  void endReset();

 private:
  bf::path dataPath_;
  XMLTreeItem* root_;

 private:
  void connectAll();
  void initDirectories() const;
  void initFromDB();
  void initFromDataDir();

  // QAbstractItemModel interface
 public:
  QVariant data(const QModelIndex& index,
                int role = Qt::DisplayRole) const override;
  QVariant headerData(int section,
                      Qt::Orientation orientation,
                      int role) const override;
  Qt::ItemFlags flags(const QModelIndex& index) const override;
  QModelIndex index(int row,
                    int column,
                    const QModelIndex& parent = QModelIndex()) const override;
  QModelIndex parent(const QModelIndex& index) const override;
  int rowCount(const QModelIndex& parent) const override;
  int columnCount(const QModelIndex& parent) const override;

  bool insertRows(int row, int count, const QModelIndex& parent) override;
  bool setData(const QModelIndex& index,
               const QVariant& value,
               int role) override;
};

#endif  // XMLTREEMODEL_H
