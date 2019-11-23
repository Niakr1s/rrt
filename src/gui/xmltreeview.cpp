#include "xmltreeview.h"

#include <QClipboard>
#include <QFileDialog>
#include <QGuiApplication>
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
  spatial_ = spatial;
  intersectsResult_.clear();
  if (spatial_->empty()) {
    emit endProcessingDXFSignal(0);
    return;
  }
  std::thread([=] {
    xmlModel()->forEach([&](XMLTreeItem* item) {
      if (item->intersects(*spatial)) {
        intersectsResult_.push_back(item->strID());
      }
    });
    emit endProcessingDXFSignal(intersectsResult_.size());
    BOOST_LOG_TRIVIAL(debug) << "XMLTreeView::onNewDXFSpatial: end, got "
                             << intersectsResult_.size() << " intersections";
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

void XMLTreeView::onCopySemicolonButtonClick() {
  QGuiApplication::clipboard()->setText(QString::fromStdString(
      VecStr<std::string>(intersectsResult_).sepBySemicolon()));
}

void XMLTreeView::onCopyNewlineButtonClick() {
  QGuiApplication::clipboard()->setText(QString::fromStdString(
      VecStr<std::string>(intersectsResult_).sepByNewLine()));
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

QVector<std::string> XMLTreeView::intersectsResult() const {
  return intersectsResult_;
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
