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
#include "vecstr.h"
#include "xml.h"
#include "xmltreedelegate.h"
#include "xmltreemodel.h"

XMLTreeView::XMLTreeView(QWidget* parent)
    : QTreeView(parent), cwd_(bf::current_path()), dataPath_(cwd_ / "data") {
  initDirectories();

  exportMenu_ = new QMenu();
  QAction* exportAction =
      new QAction(QIcon(":/icons/dxf.svg"), tr("Export to DXF"));
  exportMenu_->addAction(exportAction);

  XMLTreeModel* model = new XMLTreeModel(this);
  model_ = new XMLTreeSortFilterProxyModel(this);
  model_->setSourceModel(model);
  setModel(model_);

  auto delegate = new XMLTreeDelegate();
  setItemDelegate(delegate);

  setSortingEnabled(true);
  sortByColumn(0, Qt::SortOrder::AscendingOrder);
  setEditTriggers(QTreeView::NoEditTriggers);
  setMinimumHeight(400);

  collapseAll();

  connect(model, &XMLTreeSortFilterProxyModel::rowsInserted, this,
          &XMLTreeView::onRowsInserted);

  setContextMenuPolicy(Qt::CustomContextMenu);
  connect(this, &XMLTreeView::customContextMenuRequested, this,
          &XMLTreeView::onCustomContextMenuRequested);

  connect(exportAction, &QAction::triggered, this,
          &XMLTreeView::onExportAction);
}

void XMLTreeView::onNewDXFSpatial(std::shared_ptr<rrt::Spatial> spatial) {
  auto res = std::make_shared<DXFResult>();
  spatial_ = spatial;
  if (spatial_->empty()) {
    emit endProcessingDXFSignal(res);
    return;
  }
  std::thread([=] {
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
    model_->setFiltering(true);
    expandAll();
  }).detach();
}

void XMLTreeView::onNewXMLFiles(QVector<QFileInfo> xmlFiles) {
  int sz = xmlFiles.size();
  emit startProcessingXMLsSignal(sz);
  QVector<QString> errPaths;
  std::vector<std::thread> threads;
  for (int i = 0; i != sz; ++i) {
    try {
      auto xml = std::make_shared<rrt::XML>(
          xmlFiles[i].absoluteFilePath().toStdWString());

      xmlModel()->appendSpatials(xml->xmlSpatials());
      emit XMLtoDBStartSignal();
      std::thread([=] {
        rrt::DB::get()->pushToDB(*xml);
        emit XMLtoDBEndSignal();
      }).detach();

      try {
        bf::path newPath = xml->renameFile();
        bf::path dataPath = dataPath_ / newPath.filename();
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
    emit oneXMLProcessedSignal(i, sz);
  }

  BOOST_LOG_TRIVIAL(debug) << "XMLTreeView::onNewXMLFiles END, rootitem: ";
  xmlModel()->rootItem_->dumpInfo();
  emit endProcessingXMLsSignal(errPaths);
}

void XMLTreeView::onDxfClose() {
  model_->setFiltering(false);
  collapseAll();
  xmlModel()->forEach([](XMLTreeItem* item) { item->turnOffIntersectsFlag(); });
  spatial_ = nullptr;
}

void XMLTreeView::onRowsInserted(QModelIndex sourceParent,
                                 int first,
                                 int last) {
  for (int i = first; i <= last; ++i) {
    auto elem = model_->mapFromSource(xmlModel()->index(i, 0, sourceParent));
    expand(elem);
    QModelIndex parent = model()->index(i, 0, elem.parent());
    expandUntilRoot(parent);
  }
}

void XMLTreeView::onCustomContextMenuRequested(QPoint p) {
  if (selectedIndexes().empty()) {
    return;
  }
  exportMenu_->popup(mapToGlobal(p));
}

void XMLTreeView::onExportAction() {
  if (selectedIndexes().empty()) {
    return;
  }
  QModelIndex selected = selectedIndexes().first();
  selected = model_->mapToSource(selected);

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
  return static_cast<XMLTreeModel*>(model_->sourceModel());
}

void XMLTreeView::initDirectories() const {
  if (!bf::exists(dataPath_)) {
    bf::create_directory(dataPath_);
  }
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
