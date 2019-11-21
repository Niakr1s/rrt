#include "xmltreemodel.h"

#include <QDebug>
#include <boost/log/trivial.hpp>
#include "db.h"

XMLTreeModel::XMLTreeModel(QObject* parent) : QAbstractItemModel(parent) {
  rootItem_ = new XMLTreeItem();
  appendSpatialsFromDB();
}

XMLTreeModel::~XMLTreeModel() {
  delete rootItem_;
}

void XMLTreeModel::appendSpatials(
    const rrt::XMLSpatial::xmlSpatials_t& spatials,
    bool fromDB) {
  for (auto& spatial : spatials) {
    auto path = spatial->xmlSpatialInfo().cadastralNumber().strings();
    while (path.size() != 3) {
      path.pop_back();
    }
    path.push_back(spatial->xmlSpatialInfo().type());
    path.push_back(spatial->xmlSpatialInfo().cadastralNumber().string());

    auto idx = QModelIndex();
    for (auto& str : path) {
      bool found = false;
      for (int i = 0; i != rowCount(idx); ++i) {
        auto child = index(i, 0, idx);
        if (getItem(child)->strID() == str) {
          idx = child;
          found = true;
          break;
        }
      }
      if (!found) {
        insertRow(rowCount(idx), idx);
        idx = index(rowCount(idx) - 1, 0, idx);
        setData(idx, QString::fromStdString(str), Qt::DisplayRole);
      }
    }
    getItem(idx)->appendSpatial(spatial, fromDB);
  }
}

void XMLTreeModel::appendSpatialsFromDB() {
  auto spatials = rrt::DB::get()->getAllLastFromDB();
  appendSpatials(spatials, true);
}

int XMLTreeModel::size() const {
  int res = 0;
  getRootItem()->forEach([&res](XMLTreeItem* item) {
    if (item->spatial() != nullptr) {
      ++res;
    }
  });
  return res;
}

XMLTreeItem* XMLTreeModel::getItem(const QModelIndex& index) const {
  if (index.isValid()) {
    XMLTreeItem* item = static_cast<XMLTreeItem*>(index.internalPointer());
    if (item)
      return item;
  }
  return rootItem_;
}

XMLTreeItem* XMLTreeModel::getRootItem() const {
  return rootItem_;
}

void XMLTreeModel::forEach(std::function<void(XMLTreeItem*)> fn) {
  for (int i = 0; i != rowCount(QModelIndex()); ++i) {
    getItem(index(i, 0))->forEach(fn);
  }
}

void XMLTreeModel::forEach(QModelIndex idx,
                           std::function<void(XMLTreeItem*)> fn) {
  getItem(idx)->forEach(fn);
}

QVariant XMLTreeModel::data(const QModelIndex& index, int role) const {
  if (!index.isValid())
    return QVariant();

  XMLTreeItem* item = static_cast<XMLTreeItem*>(index.internalPointer());

  if (role == Qt::DisplayRole) {
    return item->data(0);
  }

  if (role == Qt::ToolTipRole) {
    return item->tooltipData();
  }

  return QVariant();
}

QVariant XMLTreeModel::headerData(int section,
                                  Qt::Orientation orientation,
                                  int role) const {
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    switch (section) {
      case (XMLTreeItem::Column::CadastralNumber):
        return "CadastralNumber";
      default:
        break;
    }
  return QVariant();
}

Qt::ItemFlags XMLTreeModel::flags(const QModelIndex& index) const {
  if (!index.isValid())
    return Qt::NoItemFlags;

  return QAbstractItemModel::flags(index);
}

QModelIndex XMLTreeModel::index(int row,
                                int column,
                                const QModelIndex& parent) const {
  if (!hasIndex(row, column, parent)) {
    return QModelIndex();
  }

  XMLTreeItem* parentItem;

  if (!parent.isValid()) {
    parentItem = rootItem_;
  } else {
    parentItem = static_cast<XMLTreeItem*>(parent.internalPointer());
  }

  XMLTreeItem* childItem = parentItem->child(row);
  if (childItem) {
    return createIndex(row, column, childItem);
  }

  return QModelIndex();
}

QModelIndex XMLTreeModel::parent(const QModelIndex& index) const {
  if (!index.isValid())
    return QModelIndex();

  XMLTreeItem* childItem = static_cast<XMLTreeItem*>(index.internalPointer());
  XMLTreeItem* parentItem = childItem->parentItem();

  if (parentItem == rootItem_)
    return QModelIndex();

  return createIndex(parentItem->row(), 0, parentItem);
}

int XMLTreeModel::rowCount(const QModelIndex& parent) const {
  XMLTreeItem* parentItem;
  //  if (parent.column() > 0)
  //    return 0;

  if (!parent.isValid())
    parentItem = rootItem_;
  else
    parentItem = static_cast<XMLTreeItem*>(parent.internalPointer());

  return parentItem->childCount();
}

int XMLTreeModel::columnCount(const QModelIndex& parent) const {
  if (parent.isValid())
    return static_cast<XMLTreeItem*>(parent.internalPointer())->columnCount();
  return rootItem_->columnCount();
}

bool XMLTreeModel::insertRows(int row, int count, const QModelIndex& parent) {
  XMLTreeItem* parentItem = getItem(parent);
  if (!parentItem)
    return false;

  beginInsertRows(parent, row, row + count - 1);
  const bool success =
      parentItem->insertChildren(row, count, rootItem_->columnCount());
  endInsertRows();

  return success;
}

bool XMLTreeModel::setData(const QModelIndex& index,
                           const QVariant& value,
                           int role) {
  XMLTreeItem* item = getItem(index);
  bool result = item->setData(0, value);
  if (result) {
    emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});
  }
  return result;
}
