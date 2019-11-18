#ifndef XMLTREEMODEL_H
#define XMLTREEMODEL_H

#include <QAbstractItemModel>
#include <QObject>
#include <QVariant>
#include <QVector>
#include "xmlspatial.h"
#include "xmltreeitem.h"

class XMLTreeModel : public QAbstractItemModel {
 public:
  XMLTreeModel(QObject* parent = nullptr);
  ~XMLTreeModel() override;

  void setupModelData(XMLTreeItem* parent);

 private:
  XMLTreeItem* rootItem;

  // QAbstractItemModel interface
 public:
  QVariant data(const QModelIndex& index, int role) const override;
  QVariant headerData(int section,
                      Qt::Orientation orientation,
                      int role) const override;
  Qt::ItemFlags flags(const QModelIndex& index) const override;
  QModelIndex index(int row,
                    int column,
                    const QModelIndex& parent) const override;
  QModelIndex parent(const QModelIndex& index) const override;
  int rowCount(const QModelIndex& parent) const override;
  int columnCount(const QModelIndex& parent) const override;
};

#endif  // XMLTREEMODEL_H
