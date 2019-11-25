#include "xmltreeview.h"

#include <QFileDialog>
#include <QThread>
#include <boost/date_time.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/log/trivial.hpp>
#include <exception>
#include <thread>
#include "boost/filesystem.hpp"
#include "db.h"
#include "dxf.h"
#include "xmltreedelegate.h"
#include "xmltreemodel.h"

XMLTreeView::XMLTreeView(QWidget* parent)
    : QTreeView(parent), cwd_(bf::current_path()), dataPath_(cwd_ / "data") {
  initDirectories();
  initRightClickMenu();
  initModel();

  setSortingEnabled(true);
  sortByColumn(0, Qt::SortOrder::AscendingOrder);
  setEditTriggers(QTreeView::NoEditTriggers);
  setMinimumHeight(400);
  collapseAll();

  connect(this, &XMLTreeView::customContextMenuRequested, this,
          &XMLTreeView::onCustomContextMenuRequested);

  connect(this, &XMLTreeView::endProcessingDXFSignal, this,
          &XMLTreeView::onEndProcessingDXF);

  connect(model_, &XMLTreeSortFilterProxyModel::rowsInserted, this,
          &XMLTreeView::onRowsInserted);

  connect(this, &XMLTreeView::newXMLSpatialsSignal, model_,
          &XMLTreeModel::onNewXMLSpatials);

  loadDBSpatials();
}

void XMLTreeView::onNewDXFSpatial(std::shared_ptr<rrt::Spatial> spatial) {
  spatial_ = spatial;
  if (spatial_->empty()) {
    emit endProcessingDXFSignal(std::make_shared<DXFResult>());
    return;
  }
  std::thread([=] {
    auto res = std::make_shared<DXFResult>();
    xmlModel()->forEach([&](XMLTreeItem* item) {
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
    emit endProcessingDXFSignal(res);
    BOOST_LOG_TRIVIAL(debug) << "XMLTreeView::onNewDXFSpatial: end, got "
                             << res->size() << " intersections";
  }).detach();
}

void XMLTreeView::onNewXMLFiles(QVector<QFileInfo> xmlFiles) {
  std::thread([=] {
    int sz = xmlFiles.size();
    emit startProcessingXMLsSignal(sz);
    QStringList errPaths;
    std::vector<std::thread> threads;
    for (int i = 0; i != sz; ++i) {
      auto t = std::thread([=, &errPaths, &xmlFiles] {
        try {
          auto xml = std::make_shared<rrt::XML>(
              xmlFiles[i].absoluteFilePath().toStdWString());

          emit oneXMLProcessedSignal(i, sz);
          emit newXMLSpatialsSignal(xml->xmlSpatials(), false);
          try {
            bf::path newPath = xml->renameFile();
            bf::path dataPath = dataPath_ / newPath.filename();
            if (!bf::exists(dataPath)) {
              bf::copy(newPath, dataPath);
            }
          } catch (std::exception& e) {
            BOOST_LOG_TRIVIAL(error) << e.what();
          }

          emit XMLtoDBStartSignal();
          rrt::DB::get()->pushToDB(*xml);
          emit XMLtoDBEndSignal();

        } catch (std::exception& e) {
          BOOST_LOG_TRIVIAL(error) << e.what();
          errPaths.push_back(xmlFiles[i].fileName());
        }
      });
      threads.push_back(std::move(t));
    }
    for (auto& t : threads) {
      t.join();
    }
    BOOST_LOG_TRIVIAL(debug) << "XMLTreeView::onNewXMLFiles END, rootitem: ";
    xmlModel()->rootItem_->dumpInfo();
    emit endProcessingXMLsSignal(errPaths);
  }).detach();
}

void XMLTreeView::onEndProcessingDXF(std::shared_ptr<DXFResult>) {
  proxyModel_->setFiltering(true);
  expandAll();
}

void XMLTreeView::onDxfClose() {
  proxyModel_->setFiltering(false);
  collapseAll();
  xmlModel()->forEach([](XMLTreeItem* item) { item->turnOffIntersectsFlag(); });
  spatial_ = nullptr;
}

void XMLTreeView::onRowsInserted(QModelIndex sourceParent,
                                 int first,
                                 int last) {
  for (int i = first; i <= last; ++i) {
    auto elem =
        proxyModel_->mapFromSource(xmlModel()->index(i, 0, sourceParent));
    expand(elem);
    QModelIndex parent = model()->index(i, 0, elem.parent());
    expandUntilRoot(parent);
  }
}

void XMLTreeView::onCustomContextMenuRequested(QPoint p) {
  if (selectedIndexes().empty()) {
    return;
  }
  rightClickMenu_->popup(mapToGlobal(p));
}

void XMLTreeView::onExportAction() {
  if (selectedIndexes().empty()) {
    return;
  }
  QModelIndex selected = selectedIndexes().first();
  selected = proxyModel_->mapToSource(selected);

  rrt::DXF dxf;
  if (spatial_) {
    dxf.drawSpatial(spatial_, rrt::DXF::Color::LIGHTGREEN);
  }
  xmlModel()->forEach(selected, [&](XMLTreeItem* item) {
    auto spa = item->spatial();
    if (spa != nullptr) {
      dxf.drawSpatial(spa->xmlSpatialInfo().cadastralNumber().string(),
                      spa->xmlSpatialInfo().type(), spa->spatial(),
                      spa->color());
    }
  });

  QString fileName = QFileDialog::getSaveFileName(this, tr("Save DXF File"), "",
                                                  "DXF (*.dxf)");
  auto path = boost::filesystem::path(fileName.toStdWString());
  try {
    dxf.fileExport(path);
  } catch (std::exception& e) {
    BOOST_LOG_TRIVIAL(error) << e.what();
  }
}

void XMLTreeView::onExpandButtonToggled(bool expand) {
  expand ? expandAll() : collapseAll();
}

XMLTreeModel* XMLTreeView::xmlModel() {
  return static_cast<XMLTreeModel*>(proxyModel_->sourceModel());
}

void XMLTreeView::loadDBSpatials() {
  std::thread([this] {
    BOOST_LOG_TRIVIAL(info) << "Fetching data from DB...";
    auto spatials = rrt::DB::get()->getAllLastFromDB();
    emit newXMLSpatialsSignal(spatials, true);
  }).detach();
}

void XMLTreeView::initDirectories() const {
  if (!bf::exists(dataPath_)) {
    bf::create_directory(dataPath_);
  }
}

void XMLTreeView::initModel() {
  model_ = new XMLTreeModel(this);
  proxyModel_ = new XMLTreeSortFilterProxyModel(this);
  proxyModel_->setSourceModel(model_);
  setModel(proxyModel_);
  auto delegate = new XMLTreeDelegate();
  setItemDelegate(delegate);
}

void XMLTreeView::initRightClickMenu() {
  setContextMenuPolicy(Qt::CustomContextMenu);

  rightClickMenu_ = new QMenu();
  QAction* exportAction =
      new QAction(QIcon(":/icons/dxf.svg"), tr("Export to DXF"));
  rightClickMenu_->addAction(exportAction);

  connect(exportAction, &QAction::triggered, this,
          &XMLTreeView::onExportAction);
}

void XMLTreeView::expandUntilRoot(QModelIndex item) {
  while (item.isValid()) {
    expand(item);
    item = item.parent();
  }
}

void XMLTreeView::collapseAll() {
  QTreeView::collapseAll();
  expand(QModelIndex(), 2);
}

void XMLTreeView::expand(QModelIndex idx, int count) {
  QTreeView::expand(idx);
  if (count == 0) {
    return;
  }
  for (auto row = 0; row != model()->rowCount(idx); ++row) {
    auto childIdx = model()->index(row, 0, idx);
    expand(childIdx, count - 1);
  }
}
