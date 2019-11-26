#include "mainwidget.h"

#include <QMessageBox>
#include <QStandardItemModel>
#include <QString>
#include <QVBoxLayout>
#include <boost/log/trivial.hpp>
#include "xmltreedelegate.h"
#include "xmltreemodel.h"

MainWidget::MainWidget(QWidget* parent) : QWidget(parent) {
  QVBoxLayout* vbox = new QVBoxLayout();
  treeViewBtns_ = new XMLTreeButtons();

  treeView_ = new XMLTreeView();
  dxfLabel_ = new DXFLabel();
  vbox->addWidget(treeViewBtns_);
  vbox->addWidget(treeView_);
  vbox->addWidget(dxfLabel_);
  setLayout(vbox);

  connectAll();
}

XMLTreeView* MainWidget::treeView() const {
  return treeView_;
}

DXFLabel* MainWidget::dxfLabel() const {
  return dxfLabel_;
}

void MainWidget::connectAll() {
  connect(dxfLabel_, &DXFLabel::newDXFSpatial, treeView_->xmlModel(),
          &XMLTreeModel::getIntersections);

  connect(dxfLabel_, &DXFLabel::dxfClosed, treeView_,
          &XMLTreeView::disableIntersectionsFiltering);
  connect(dxfLabel_, &DXFLabel::dxfClosed, treeViewBtns_,
          &XMLTreeButtons::hideCopyElements);

  connect(dxfLabel_, &DXFLabel::newXMLs, treeView_->xmlModel(),
          &XMLTreeModel::appendXMLs);

  connect(treeViewBtns_->btnExpand(), &QPushButton::toggled, treeView_,
          &XMLTreeView::expandIf);
  connect(treeViewBtns_->btnExpand(), &QPushButton::toggled, treeView_,
          &XMLTreeView::expandIf);

  connect(treeView_->xmlModel(), &XMLTreeModel::gotIntersections, dxfLabel_,
          &DXFLabel::onEndProcessingDXFSignal, Qt::QueuedConnection);
  connect(treeView_->xmlModel(), &XMLTreeModel::gotIntersections, treeViewBtns_,
          &XMLTreeButtons::activateCopyElements);
}
