#include "xmltreemodel.h"

#include <boost/filesystem.hpp>
#include <boost/log/trivial.hpp>
#include <thread>
#include "db.h"
#include "util.h"

XMLTreeModel::XMLTreeModel(QObject* parent)
    : QAbstractItemModel(parent), dataPath_(bf::current_path() / DATA_PATH) {
  BOOST_LOG_TRIVIAL(debug) << "XMLTreeModel::XMLTreeModel";
  root_ = XMLRootTreeItem::get();
  initDirectories();
  connectAll();
  initFromDB();
}

// deprecated
void XMLTreeModel::appendSpatialsSlow(const rrt::xmlSpatials_t& spatials,
                                      bool fromDB) {
  BOOST_LOG_TRIVIAL(debug) << "XMLTreeModel::appendSpatials: spatials = "
                           << spatials.size() << ", fromDB = " << fromDB;
  for (auto& spatial : spatials) {
    auto path = preparePath(spatial);

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

void XMLTreeModel::appendSpatials(const rrt::xmlSpatials_t& spatials,
                                  bool fromDB) {
  int len = static_cast<int>(spatials.size());
  emit startProcessingSignal(len);
  beginResetModel();
  for (int i = 0; i != len; ++i) {
    XMLRootTreeItem::appendSpatial(spatials[static_cast<size_t>(i)], fromDB);
    emit oneProcessedSignal(i, len);
  }
  endResetModel();
  emit endProcessingSignal();
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
  return XMLRootTreeItem::get();
}

XMLTreeItem* XMLTreeModel::getRootItem() const {
  return XMLRootTreeItem::get();
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

void XMLTreeModel::onXmlTreeItemDataChanged(XMLTreeItem* item) {}

void XMLTreeModel::appendXMLs(QVector<QFileInfo> xmlFiles) {
  BOOST_LOG_TRIVIAL(debug) << "XMLTreeModel::onNewXMLFiles: got "
                           << xmlFiles.size() << " files";
  std::thread([=] {
    int sz = xmlFiles.size();
    //    emit startProcessingXMLsSignal(sz);
    QStringList errPaths;
    rrt::xmlSpatials_t allSpatials;
    std::vector<std::thread> threads;
    for (int i = 0; i != sz; ++i) {
      try {
        auto xml = std::make_shared<rrt::XML>(
            xmlFiles[i].absoluteFilePath().toStdWString());
        auto xmlSpatials = xml->xmlSpatials();
        allSpatials.insert(allSpatials.end(), xmlSpatials.begin(),
                           xmlSpatials.end());
        //        emit oneXMLProcessedSignal(i, sz);
        try {
          bf::path newPath = xml->renameFile();
          bf::path dataPath = dataPath_ / newPath.filename();
          if (!bf::exists(dataPath)) {
            bf::copy(newPath, dataPath);
          }
        } catch (std::exception& e) {
          BOOST_LOG_TRIVIAL(error) << e.what();
        }

        auto t = std::thread([=] {
          emit DBBeginSignal();
          rrt::DB::get()->pushToDB(*xml);
          emit DBEndSignal();
        });
        threads.push_back(std::move(t));

      } catch (std::exception& e) {
        BOOST_LOG_TRIVIAL(error) << e.what();
        errPaths.push_back(xmlFiles[i].fileName());
      }
    }
    emit newXMLSpatialsSignal(allSpatials, false);

    for (auto& t : threads) {
      t.join();
    }
    emit endProcessingXMLsSignal(errPaths);
    BOOST_LOG_TRIVIAL(debug)
        << "XMLTreeView::onNewXMLFiles: got " << errPaths.size() << " errors";
  }).detach();
}

void XMLTreeModel::onNewXMLSpatials(rrt::xmlSpatials_t spatials, bool fromDB) {
  appendSpatials(spatials, fromDB);
}

void XMLTreeModel::endReset() {
  endResetModel();
}

void XMLTreeModel::connectAll() {
  connect(this, &XMLTreeModel::appendSpatialsFinishedSignal, this,
          &XMLTreeModel::endReset);
  connect(this, &XMLTreeModel::newXMLSpatialsSignal, this,
          &XMLTreeModel::onNewXMLSpatials);
}

void XMLTreeModel::initDirectories() const {
  if (!bf::exists(dataPath_)) {
    bf::create_directory(dataPath_);
  }
}

void XMLTreeModel::initFromDB() {
  std::thread([this] {
    auto spatials = rrt::DB::get()->getAllLastFromDB();
    emit newXMLSpatialsSignal(spatials, true);
  }).detach();
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
    parentItem = XMLRootTreeItem::get();
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

  if (parentItem == XMLRootTreeItem::get() || parentItem == nullptr)
    return QModelIndex();

  return createIndex(parentItem->row(), 0, parentItem);
}

int XMLTreeModel::rowCount(const QModelIndex& parent) const {
  XMLTreeItem* parentItem;
  //  if (parent.column() > 0)
  //    return 0;

  if (!parent.isValid())
    parentItem = XMLRootTreeItem::get();
  else
    parentItem = static_cast<XMLTreeItem*>(parent.internalPointer());

  return parentItem->childCount();
}

int XMLTreeModel::columnCount(const QModelIndex& parent) const {
  if (parent.isValid())
    return static_cast<XMLTreeItem*>(parent.internalPointer())->columnCount();
  return XMLRootTreeItem::get()->columnCount();
}

bool XMLTreeModel::insertRows(int row, int count, const QModelIndex& parent) {
  XMLTreeItem* parentItem = getItem(parent);
  if (!parentItem)
    return false;

  beginInsertRows(parent, row, row + count - 1);
  const bool success = parentItem->insertChildren(
      row, count, XMLRootTreeItem::get()->columnCount());
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
