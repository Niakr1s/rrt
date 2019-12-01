#include "xmltreemodel.h"

#include <QDir>
#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/log/trivial.hpp>
#include <thread>
#include "xml.h"

namespace bf = boost::filesystem;

XMLTreeModel::XMLTreeModel(QObject* parent)
    : QAbstractItemModel(parent), dataPath_(L"data") {
  root_ = XMLRootTreeItem::get();
  initDirectories();
  connectAll();
  initFromDB();
}

void XMLTreeModel::appendSpatials(const rrt::xmlSpatials_t& spatials,
                                  bool fromDB) {
  BOOST_LOG_TRIVIAL(info) << "Starting to append " << spatials.size()
                          << " spatials";
  std::thread([=] {
    int len = static_cast<int>(spatials.size());
    emit startProcessing(len);
    beginResetModel();
    for (int i = 0; i != len; ++i) {
      XMLRootTreeItem::appendSpatial(spatials[static_cast<size_t>(i)], fromDB);
      emit oneProcessed(i, len);
    }
    endResetModel();
    emit endProcessing();
  }).detach();
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

bool XMLTreeModel::anyChildIntersectsFlag(const QModelIndex& idx) const {
  auto item = getItem(idx);
  return item->anyChildIntersectsFlag();
}

void XMLTreeModel::forEach(std::function<void(XMLTreeItem*)> fn,
                           bool onlyIntersected) {
  for (int i = 0; i != rowCount(QModelIndex()); ++i) {
    getItem(index(i, 0))->forEach(fn, onlyIntersected);
  }
}

void XMLTreeModel::forEach(QModelIndex idx,
                           std::function<void(XMLTreeItem*)> fn,
                           bool onlyIntersected) {
  getItem(idx)->forEach(fn, onlyIntersected);
}

void XMLTreeModel::appendXMLs(QVector<QFileInfo> xmlFiles, bool fromDB) {
  BOOST_LOG_TRIVIAL(info) << "Appending " << xmlFiles.size() << " files ...";
  std::thread([=] {
    int sz = xmlFiles.size();
    QStringList errPaths;
    rrt::xmlSpatials_t allSpatials;
    for (int i = 0; i != sz; ++i) {
      try {
        auto xml = std::make_shared<rrt::XML>(
            xmlFiles[i].absoluteFilePath().toStdWString());
        auto xmlSpatials = xml->xmlSpatials();
        allSpatials.insert(allSpatials.end(), xmlSpatials.begin(),
                           xmlSpatials.end());
        try {
          bf::path newPath = xml->renameFile();
          bf::path dataPath = bf::path(dataPath_) / newPath.filename();
          if (!bf::exists(dataPath)) {
            bf::copy(newPath, dataPath);
          }
        } catch (std::exception& e) {
          BOOST_LOG_TRIVIAL(error) << e.what();
        }

      } catch (std::exception& e) {
        BOOST_LOG_TRIVIAL(error) << e.what();
        errPaths.push_back(xmlFiles[i].fileName());
      }
    }
    emit newXMLSpatials(allSpatials, fromDB);
    emit endAppendingXMLs(errPaths);
    BOOST_LOG_TRIVIAL(info)
        << "Appended " << (xmlFiles.size() - errPaths.size()) << ", got "
        << errPaths.size() << " errors";
  }).detach();
}

void XMLTreeModel::getIntersections(std::shared_ptr<rrt::Spatial> spatial) {
  BOOST_LOG_TRIVIAL(debug) << "XMLTreeModel::getIntersections: begin";
  if (spatial->empty()) {
    BOOST_LOG_TRIVIAL(debug)
        << "XMLTreeModel::getIntersections: spatial is nullptr";
    emit gotIntersections(std::make_shared<DXFResult>());
    return;
  }
  BOOST_LOG_TRIVIAL(debug) << "XMLTreeModel::getIntersections: processing...";
  std::thread([=] {
    auto res = std::make_shared<DXFResult>();
    forEach([&](XMLTreeItem* item) {
      if (item->intersects(*spatial)) {
        auto key = item->parentItem()->qstrID();
        (*res)[key].push_back(item->qstrID());
      }
    });
    for (auto& key : res->keys()) {
      qSort((*res)[key].begin(), (*res)[key].end(),
            [](const QString& lhs, const QString& rhs) {
              return lhs.split(":").back().toInt() <
                     rhs.split(":").back().toInt();
            });
    }
    emit gotIntersections(res);
    BOOST_LOG_TRIVIAL(debug) << "XMLTreeModel::getIntersections: end";
  }).detach();
}

void XMLTreeModel::exportToDXF(QModelIndex idx, QString fileName) {
  std::thread([=] {
    rrt::DXF dxf;
    if (spatial_) {
      dxf.drawSpatial(spatial_, rrt::DXF::Color::LIGHTGREEN);
    }
    forEach(
        idx,
        [&](XMLTreeItem* item) {
          auto spa = item->spatial();
          if (spa != nullptr) {
            dxf.drawSpatial(spa->xmlSpatialInfo().cadastralNumber().string(),
                            spa->xmlSpatialInfo().type(), spa->spatial(),
                            spa->color());
          }
        },
        anyChildIntersectsFlag(idx));

    auto path = boost::filesystem::path(fileName.toStdWString());

    try {
      dxf.fileExport(path);
    } catch (std::exception& e) {
      BOOST_LOG_TRIVIAL(error) << "Error while export to DXF: " << e.what();
      emit DXFExportDone(tr("DXF export failure"), e.what(),
                         QMessageBox::Icon::Critical);
    }
  }).detach();
}

void XMLTreeModel::endReset() {
  endResetModel();
}

void XMLTreeModel::connectAll() {
  connect(this, &XMLTreeModel::newXMLSpatials, this,
          &XMLTreeModel::appendSpatials);
  connect(this, &XMLTreeModel::newXMLs, this, &XMLTreeModel::appendXMLs);
}

void XMLTreeModel::initDirectories() const {
  if (!bf::exists(dataPath_)) {
    bf::create_directory(dataPath_);
  }
}

void XMLTreeModel::initFromDB() {
  BOOST_LOG_TRIVIAL(info) << "Starting init from DB ...";
  std::thread([this] {
    QVector<QFileInfo> found;
    QDir dir(QString::fromStdWString(dataPath_));
    auto list = dir.entryInfoList();
    for (auto& entry : list) {
      if (entry.isFile() && entry.suffix().toLower() == "xml") {
        found.push_back(entry);
      }
    }
    emit newXMLs(found, true);
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
