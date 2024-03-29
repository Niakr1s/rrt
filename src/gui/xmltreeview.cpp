#include "xmltreeview.h"

#include <QApplication>
#include <QClipboard>
#include <QDesktopServices>
#include <QFileDialog>
#include <QList>
#include <QMessageBox>
#include <QMimeData>
#include <QMouseEvent>
#include <QThread>
#include <QUrl>
#include <boost/date_time.hpp>
#include <boost/log/trivial.hpp>
#include <exception>
#include <thread>
#include <unordered_set>
#include "dxf.h"
#include "xmltreedelegate.h"
#include "xmltreemodel.h"

XMLTreeView::XMLTreeView(QWidget* parent) : QTreeView(parent) {
  initRightClickMenu();

  setSortingEnabled(true);
  sortByColumn(0, Qt::SortOrder::AscendingOrder);
  setEditTriggers(QTreeView::NoEditTriggers);
  setMinimumHeight(400);

  initModel();  // should be after connectAll

  connectAll();
}

void XMLTreeView::showIntersected(std::shared_ptr<DXFResult>) {
  proxyModel_->setFiltering(true);
  expandAll();
}

void XMLTreeView::disableIntersectionsFiltering() {
  proxyModel_->setFiltering(false);
  collapseAll();
  xmlModel()->forEach([](XMLTreeItem* item) { item->turnOffIntersectsFlag(); },
                      true);
}

void XMLTreeView::expandChildsUntilRoot(QModelIndex sourceParent,
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

void XMLTreeView::showCustomContextMenu(QPoint p) {
  if (selectedIndexes().empty()) {
    return;
  }
  rightClickMenu_->popup(mapToGlobal(p));
}

void XMLTreeView::exportSelectedToDXF() {
  if (selectedIndexes().empty()) {
    return;
  }
  QModelIndex selected = selectedIndexes().first();
  selected = proxyModel_->mapToSource(selected);
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save DXF File"), "",
                                                  "DXF (*.dxf)");
  emit exportToDXF(selected, fileName);
}

void XMLTreeView::copyXMLsToClipboard() {
  if (selectedIndexes().empty()) {
    return;
  }
  QModelIndex selected = selectedIndexes().first();
  selected = proxyModel_->mapToSource(selected);

  std::unordered_set<std::wstring> set;
  xmlModel()->forEach(
      selected,
      [&set](XMLTreeItem* item) {
        if (item->hasSpatial()) {
          auto p = item->spatial()->xmlInfo().path().wstring();
          set.insert(p);
        }
      },
      xmlModel()->anyChildIntersectsFlag(selected));

  QList<QUrl> urls;
  for (auto& url : set) {
    auto qstr = QString::fromStdWString(url);
    urls.push_back(QUrl::fromLocalFile(qstr));
  }

  QMimeData* mime = new QMimeData();
  mime->setUrls(urls);
  QApplication::clipboard()->setMimeData(mime);
  BOOST_LOG_TRIVIAL(debug) << "Copied " << urls.size() << " items to clipboard";
}

void XMLTreeView::showErrXMLsMessageBox(QStringList errXMLPaths) {
  if (!errXMLPaths.empty()) {
    QMessageBox* errXmlMessageBox = new QMessageBox(this);
    errXmlMessageBox->setMinimumWidth(600);
    errXmlMessageBox->setWindowTitle(tr("Couldn't parse some XMLs"));
    errXmlMessageBox->setText(tr("Invalid file names are listed below"));
    errXmlMessageBox->setIcon(QMessageBox::Warning);
    errXmlMessageBox->setDetailedText(errXMLPaths.join("\n"));
    errXmlMessageBox->show();
  }
}

void XMLTreeView::showMessageBox(QString title,
                                 QString msg,
                                 QMessageBox::Icon icon) {
  QMessageBox* errXmlMessageBox = new QMessageBox(this);
  errXmlMessageBox->setWindowTitle(title);
  errXmlMessageBox->setText(msg);
  errXmlMessageBox->setIcon(icon);
  errXmlMessageBox->show();
}

void XMLTreeView::expandIf(bool expand) {
  expand ? expandAll() : collapseAll();
}

XMLTreeModel* XMLTreeView::xmlModel() {
  return static_cast<XMLTreeModel*>(proxyModel_->sourceModel());
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
  exportAction_ = new QAction(QIcon(":/icons/dxf.svg"), tr("Export to DXF"));
  copyXMLsToClipboardAction_ = new QAction(QIcon(":/icons/clipboard.svg"),
                                           tr("Copy XML files to clipboard"));
  rightClickMenu_->addAction(exportAction_);
  rightClickMenu_->addAction(copyXMLsToClipboardAction_);
}

void XMLTreeView::connectAll() {
  connect(this, &XMLTreeView::customContextMenuRequested, this,
          &XMLTreeView::showCustomContextMenu);

  connect(xmlModel(), &XMLTreeModel::gotIntersections, this,
          &XMLTreeView::showIntersected);

  connect(model_, &XMLTreeSortFilterProxyModel::rowsInserted, this,
          &XMLTreeView::expandChildsUntilRoot);

  connect(exportAction_, &QAction::triggered, this,
          &XMLTreeView::exportSelectedToDXF);
  connect(copyXMLsToClipboardAction_, &QAction::triggered, this,
          &XMLTreeView::copyXMLsToClipboard);

  connect(model_, &XMLTreeModel::endProcessing, this,
          &XMLTreeView::collapseAll);

  connect(this, &XMLTreeView::exportToDXF, xmlModel(),
          &XMLTreeModel::exportToDXF);

  connect(xmlModel(), &XMLTreeModel::endAppendingXMLs, this,
          &XMLTreeView::showErrXMLsMessageBox);

  connect(xmlModel(), &XMLTreeModel::DXFExportDone, this,
          &XMLTreeView::showMessageBox);
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

void XMLTreeView::mouseDoubleClickEvent(QMouseEvent* event) {
  auto idx = indexAt(event->pos());
  auto item = XMLTreeModel().getItem(proxyModel_->mapToSource(idx));
  if (item == nullptr) {
    event->ignore();
    return;
  }
  if (item->hasSpatial()) {
    auto p = item->spatial()->xmlInfo().path();
    auto url = QUrl::fromLocalFile(QString::fromStdWString(p.wstring()));
    QDesktopServices::openUrl(url);
    event->accept();
  }
  event->ignore();
}
