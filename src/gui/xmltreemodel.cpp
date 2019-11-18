#include "xmltreemodel.h"

#include "db.h"

XMLTreeModel::XMLTreeModel(QObject* parent) : QAbstractItemModel(parent) {
  rootItem = new XMLTreeItem();
  setupModelData(rootItem);
}

XMLTreeModel::~XMLTreeModel() {
  delete rootItem;
}

void XMLTreeModel::setupModelData(XMLTreeItem* parent) {
  auto spatials = rrt::DB::get()->getAllLastFromDB();

  for (auto& spatial : spatials) {
    auto path = spatial->xmlSpatialInfo().cadastralNumber().strings();
    while (path.size() != 3) {
      path.pop_back();
    }
    path.push_back(spatial->xmlSpatialInfo().type());
    path.push_back(spatial->xmlSpatialInfo().cadastralNumber().string());

    XMLTreeItem* it = parent;
    for (auto& str : path) {
      XMLTreeItem* child = new XMLTreeItem(str, it);
      auto idChild = it->child(str);

      // second condition: we want all versions of a spatial with one
      // cadastral number
      if (idChild == nullptr || str == path.back()) {
        it->appendChild(child);
        it = child;
      } else {
        it = idChild;
      }
    }
    it->appendSpatial(spatial);
  }
}

QVariant XMLTreeModel::data(const QModelIndex& index, int role) const {
  if (!index.isValid())
    return QVariant();

  XMLTreeItem* item = static_cast<XMLTreeItem*>(index.internalPointer());

  if (role == Qt::DisplayRole) {
    return item->data(index.column());
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
    parentItem = rootItem;
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

  if (parentItem == rootItem)
    return QModelIndex();

  return createIndex(parentItem->row(), 0, parentItem);
}

int XMLTreeModel::rowCount(const QModelIndex& parent) const {
  XMLTreeItem* parentItem;
  if (parent.column() > 0)
    return 0;

  if (!parent.isValid())
    parentItem = rootItem;
  else
    parentItem = static_cast<XMLTreeItem*>(parent.internalPointer());

  return parentItem->childCount();
}

int XMLTreeModel::columnCount(const QModelIndex& parent) const {
  if (parent.isValid())
    return static_cast<XMLTreeItem*>(parent.internalPointer())->columnCount();
  return rootItem->columnCount();
}
