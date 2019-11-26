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

  connectAll();
}

void XMLTreeView::onNewDXFSpatial(std::shared_ptr<rrt::Spatial> spatial) {
  BOOST_LOG_TRIVIAL(debug) << "XMLTreeView::onNewDXFSpatial";
  spatial_ = spatial;
  if (spatial_->empty()) {
    BOOST_LOG_TRIVIAL(debug)
        << "XMLTreeView::onNewDXFSpatial: spatial is nullptr";
    emit endProcessingDXFSignal(std::make_shared<DXFResult>());
    return;
  }
  BOOST_LOG_TRIVIAL(debug) << "XMLTreeView::onNewDXFSpatial: processing...";
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
    BOOST_LOG_TRIVIAL(debug)
        << "XMLTreeView::onNewDXFSpatial: done processing dxf";
  }).detach();
}

void XMLTreeView::onNewXMLFiles(QVector<QFileInfo> xmlFiles) {
  BOOST_LOG_TRIVIAL(debug) << "XMLTreeView::onNewXMLFiles: got "
                           << xmlFiles.size() << " files";
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

          emit DBBeginSignal();
          rrt::DB::get()->pushToDB(*xml);
          emit DBEndSignal();

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
    emit endProcessingXMLsSignal(errPaths);
    BOOST_LOG_TRIVIAL(debug)
        << "XMLTreeView::onNewXMLFiles: got " << errPaths.size() << " errors";
  }).detach();
}

void XMLTreeView::onEndAppendingXMLs() {
  collapseAll();
}

void XMLTreeView::onEndProcessingDXF(std::shared_ptr<DXFResult>) {
  BOOST_LOG_TRIVIAL(debug) << "XMLTreeView::onEndProcessingDXF";
  proxyModel_->setFiltering(true);
  expandAll();
}

void XMLTreeView::onDxfClose() {
  BOOST_LOG_TRIVIAL(debug) << "XMLTreeView::onDxfClose";
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
  BOOST_LOG_TRIVIAL(debug) << "XMLTreeView::onExportAction";
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
    BOOST_LOG_TRIVIAL(error) << "Error while export to DXF: " << e.what();
  }
}

void XMLTreeView::onExpandButtonToggled(bool expand) {
  expand ? expandAll() : collapseAll();
}

void XMLTreeView::onNewXMLSpatials(rrt::xmlSpatials_t, bool fromDB) {
  //  if (!fromDB) {
  //    return;
  //  }
  //  collapseAll();
}

XMLTreeModel* XMLTreeView::xmlModel() {
  return static_cast<XMLTreeModel*>(proxyModel_->sourceModel());
}

void XMLTreeView::initDirectories() const {
  if (!bf::exists(dataPath_)) {
    bf::create_directory(dataPath_);
  }
}

void XMLTreeView::initModel() {
  BOOST_LOG_TRIVIAL(debug) << "XMLTreeView::initModel";
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
  exportAction_ = new QAction(QIcon(":/icons/dxf.svg"), tr("Export to DXF"));
  rightClickMenu_->addAction(exportAction_);
}

void XMLTreeView::connectAll() {
  connect(this, &XMLTreeView::customContextMenuRequested, this,
          &XMLTreeView::onCustomContextMenuRequested);

  connect(this, &XMLTreeView::endProcessingDXFSignal, this,
          &XMLTreeView::onEndProcessingDXF);

  connect(model_, &XMLTreeSortFilterProxyModel::rowsInserted, this,
          &XMLTreeView::onRowsInserted);

  connect(this, &XMLTreeView::newXMLSpatialsSignal, model_,
          &XMLTreeModel::onNewXMLSpatials);

  connect(exportAction_, &QAction::triggered, this,
          &XMLTreeView::onExportAction);

  connect(this, &XMLTreeView::newXMLSpatialsSignal, this,
          &XMLTreeView::onNewXMLSpatials);

  connect(model_, &XMLTreeModel::newXMLSpatialsSignal, this,
          &XMLTreeView::onNewXMLSpatials);

  connect(model_, &XMLTreeModel::startProcessingSignal, this,
          &XMLTreeView::startProcessingXMLsSignal);
  connect(model_, &XMLTreeModel::oneProcessedSignal, this,
          &XMLTreeView::oneXMLProcessedSignal);

  connect(model_, &XMLTreeModel::DBBeginSignal, this,
          &XMLTreeView::DBBeginSignal);
  connect(model_, &XMLTreeModel::DBEndSignal, this, &XMLTreeView::DBEndSignal);

  connect(model_, &XMLTreeModel::endProcessingSignal, this,
          &XMLTreeView::onEndAppendingXMLs);
}

void XMLTreeView::expandUntilRoot(QModelIndex item) {
  while (item.isValid()) {
    expand(item);
    item = item.parent();
  }
}

void XMLTreeView::collapseAll() {
  QTreeView::collapseAll();
  expand(QModelIndex(), 1);
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
