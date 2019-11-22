#include "xmltreeview.h"

#include <QClipboard>
#include <QFileDialog>
#include <QGuiApplication>
#include <QThread>
#include <boost/date_time.hpp>
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

  XMLTreeModel* model = new XMLTreeModel();
  setModel(model);

  auto delegate = new XMLTreeDelegate();
  setItemDelegate(delegate);

  setSortingEnabled(true);
  setEditTriggers(QTreeView::NoEditTriggers);
  setMinimumHeight(400);

  expandAll();

  connect(model, &XMLTreeModel::rowsInserted, this,
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
  std::thread([=] {
    int sz = xmlFiles.size();
    emit startProcessingXMLsSignal(sz);
    QVector<QString> errPaths;
    std::vector<std::thread> threads;
    for (int i = 0; i != sz; ++i) {
      auto t = std::thread([&errPaths, &xmlFiles, sz, i, this] {
        try {
          rrt::XML xml(xmlFiles[i].absoluteFilePath().toStdWString());

          xmlModel()->mutex_.lock();
          static_cast<XMLTreeModel*>(model())->appendSpatials(
              xml.xmlSpatials());
          xmlModel()->mutex_.unlock();
          emit oneXMLProcessedSignal(i, sz);

          rrt::DB::get()->pushToDB(xml);
          try {
            bf::path newPath = xml.renameFile();
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
      });
      threads.push_back(std::move(t));
    }
    for (auto& t : threads) {
      t.join();
    }
    emit endProcessingXMLsSignal(errPaths);
  }).detach();
}

void XMLTreeView::onDxfClose() {
  xmlModel()->forEach([](XMLTreeItem* item) { item->turnOffIntersectsFlag(); });
  spatial_ = nullptr;
}

void XMLTreeView::onRowsInserted(QModelIndex parent, int first, int last) {
  for (int i = first; i <= last; ++i) {
    expand(model()->index(i, 0, parent));
    QModelIndex parentInner = parent;
    expandUntilRoot(parentInner);
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
  try {
    dxf.fileExport(fileName.toStdString());
  } catch (std::exception& e) {
    BOOST_LOG_TRIVIAL(error) << e.what();
  }
}

void XMLTreeView::onExpandButtonToggled(bool expand) {
  expand ? expandAll() : collapseAll();
}

XMLTreeModel* XMLTreeView::xmlModel() {
  return static_cast<XMLTreeModel*>(model());
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
