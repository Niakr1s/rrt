#ifndef XMLTREEMODEL_H
#define XMLTREEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QObject>
#include <QVariant>
#include <QVector>
#include "xmlspatial.h"
#include "xmltreeitem.h"

class XMLTreeModel : public QAbstractItemModel {
 public:
  XMLTreeModel(QObject* parent = nullptr);
  ~XMLTreeModel() override;

  void appendSpatials(const rrt::XMLSpatial::xmlSpatials_t& spatials);
  void appendSpatialsFromDB();
  int size() const;

  XMLTreeItem* getItem(const QModelIndex& index) const;
  XMLTreeItem* getRootItem() const;

 private:
  XMLTreeItem* rootItem_;

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
