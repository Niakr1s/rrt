#include "xmltreeview.h"

#include <boost/date_time.hpp>
#include <boost/log/trivial.hpp>
#include <exception>
#include <future>
#include "boost/filesystem.hpp"
#include "db.h"
#include "xml.h"
#include "xmltreedelegate.h"
#include "xmltreemodel.h"

XMLTreeView::XMLTreeView(QWidget* parent)
    : QTreeView(parent), cwd_(bf::current_path()), dataPath_(cwd_ / "data") {
  initDirectories();

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
}

void XMLTreeView::onNewDXFSpatial(std::shared_ptr<rrt::Spatial> spatial) {
  intersectsResult_.clear();
  std::async([&] {
    xmlModel()->forEach([&](XMLTreeItem* item) {
      if (item->intersects(*spatial)) {
        intersectsResult_.push_back(item->strID());
      }
    });
  });
}

void XMLTreeView::onNewXMLFiles(QVector<QFileInfo> xmlFiles) {
  QVector<QString> errPaths;
  for (auto& xmlFile : xmlFiles) {
    if (!xmlFile.exists()) {
      continue;
    }

    try {
      rrt::XML xml(xmlFile.absoluteFilePath().toStdWString());
      static_cast<XMLTreeModel*>(model())->appendSpatials(xml.xmlSpatials());
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
      errPaths.push_back(xmlFile.fileName());
    }
  }
  if (!errPaths.empty()) {
    errXMLsSignal(errPaths);
  }
}

void XMLTreeView::onDxfClose() {
  xmlModel()->forEach([](XMLTreeItem* item) { item->turnOffIntersectsFlag(); });
}

void XMLTreeView::onRowsInserted(const QModelIndex& parent,
                                 int first,
                                 int last) {
  BOOST_LOG_TRIVIAL(debug) << " XMLTreeView::onRowsInserted";
  for (int i = first; i <= last; ++i) {
    expand(model()->index(i, 0, parent));
  }
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
