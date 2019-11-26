#ifndef XMLTREEMODEL_H
#define XMLTREEMODEL_H

#include <QAbstractItemModel>
#include <QFileInfo>
#include <QModelIndex>
#include <QObject>
#include <QVariant>
#include <QVector>
#include <string>
#include "typedefs.h"
#include "xmlroottreeitem.h"
#include "xmlspatial.h"
#include "xmltreeitem.h"

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
  void newXMLs(QVector<QFileInfo> xmlFiles, bool fromDB);
  void newXMLSpatials(rrt::xmlSpatials_t spatials, bool fromDB);
  void startProcessing(int size);
  void oneProcessed(int pos, int max);
  void endProcessing();
  void DBBegin();
  void DBEnd();
  void endAppendingXMLs(QStringList err);
  void gotIntersections(std::shared_ptr<DXFResult>);

 public slots:
  void appendXMLs(QVector<QFileInfo> xmlFiles, bool fromDB);
  void onNewXMLSpatials(rrt::xmlSpatials_t spatials, bool fromDB);
  void getIntersections(std::shared_ptr<rrt::Spatial> spatial);
  void exportToDXF(QModelIndex idx, QString fileName);
  void endReset();

 private:
  std::wstring dataPath_;
  XMLTreeItem* root_;
  std::shared_ptr<rrt::Spatial> spatial_ = std::make_shared<rrt::Spatial>();

 private:
  void connectAll();
  void initDirectories() const;
#ifdef WITH_DB
  void initFromDB();
#else
  void initFromDB();
#endif

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
